

#include "PileupMerge.h"
#include "CaloHit/CaloHitConverter.h"
#include "EventInfo/EventInfoConverter.h"


using namespace SG;
using namespace Gaugi;



PileupMerge::PileupMerge( std::string name ) : 
  IMsgService(name),
  Algorithm(),
  m_rng(0)
{
  declareProperty( "InputFile"          , m_inputFile=""                       );
  declareProperty( "InputHitsKey"       , m_inputHitsKey="Hits"                );
  declareProperty( "OutputHitsKey"      , m_outputHitsKey="Hits_Merged"        );
  declareProperty( "InputEventKey"      , m_inputEventKey="EventInfo"          );
  declareProperty( "OutputEventKey"     , m_outputEventKey="EventInfo_Merged"  );
  declareProperty( "OutputLevel"        , m_outputLevel=1                      );
  declareProperty( "NtupleName"         , m_ntupleName="CollectionTree"        );
  declareProperty( "Seed"               , m_seed=0                             );
}

//!=====================================================================

PileupMerge::~PileupMerge()
{}

//!=====================================================================

StatusCode PileupMerge::initialize()
{
  CHECK_INIT();
  setMsgLevel(m_outputLevel);
  m_rng.SetSeed(m_seed);
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode PileupMerge::finalize()
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode PileupMerge::bookHistograms( EventContext &ctx ) const
{
  auto store = ctx.getStoreGateSvc();
  auto file = new TFile(m_inputFile.c_str(), "read") ;
  store->decorate( "minbias", file );
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode PileupMerge::pre_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode PileupMerge::execute( EventContext &/*ctx*/, const G4Step * /*step*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode PileupMerge::execute( EventContext &ctx, int /*evt*/ ) const
{
  return post_execute(ctx);
}

//!=====================================================================

StatusCode PileupMerge::post_execute( EventContext &ctx ) const
{

  SG::ReadHandle<xAOD::CaloHitContainer> container(m_inputHitsKey, ctx);
  if( !container.isValid() )
  {
    MSG_FATAL("It's not possible to read the xAOD::CaloHitContainer from this Contaxt using this key " << m_inputHitsKey );
  }


  std::map<unsigned int, xAOD::CaloHit*> hit_map;

  MSG_INFO( "Convert hits to hash map...");
  for( const auto& const_hit : **container.ptr())
  {
    // Create the truth cell 
    auto hit = new xAOD::CaloHit( const_hit->eta(),
                                  const_hit->phi(),
                                  const_hit->deltaEta(),
                                  const_hit->deltaPhi(),
                                  const_hit->hash(),
                                  const_hit->sampling(),
                                  const_hit->detector(),
                                  const_hit->bc_duration(),
                                  const_hit->bcid_start(),
                                  const_hit->bcid_end()
                                  );

    for ( int bcid = hit->bcid_start();  bcid <= hit->bcid_end(); ++bcid )
    {
      hit->edep( bcid, const_hit->edep(bcid) ); // truth energy for each bunch crossing
    }

    hit_map.insert( std::make_pair(hit->hash(), hit) );
  }
  

  MSG_DEBUG( "Link all branches..." );
  auto store = ctx.getStoreGateSvc();
  auto file = (TFile*)store->decorator("minbias");
  auto tree = (TTree*)file->Get(m_ntupleName.c_str());

  std::vector<xAOD::CaloHit_t> *collection_hits=nullptr;
  InitBranch( tree,  ("CaloHitContainer_"+m_inputHitsKey).c_str(), &collection_hits );
  std::vector<xAOD::EventInfo_t> *collection_events=nullptr;
  InitBranch( tree,  ("EventInfoContainer_"+m_inputEventKey).c_str(), &collection_events );



  MSG_DEBUG( "Sort a new event...");
  int eventNumber = m_rng.Integer(tree->GetEntries()-1);

  if (tree->GetEntry( eventNumber ) < 0){
    MSG_FATAL("Not possible to read this event. repeat...");
  }

  float nPileup = collection_events->at(0).avgmu;

  // merge the energy for each hit
  for (const auto& hit_t : *collection_hits )
  {
    if ( hit_map.count(hit_t.hash)){
      
      auto hit = hit_map.at(hit_t.hash);
      int pos=0;
      for ( int bcid = hit->bcid_start();  bcid <= hit->bcid_end(); ++bcid)
      {
        hit->edep( bcid, hit_t.edep.at(pos) ); // merge
        pos++;
      }
    }
  }

  MSG_INFO( "Writing new container hits...");

  {
    SG::WriteHandle<xAOD::CaloHitContainer> hits(m_outputHitsKey, ctx);
    hits.record( std::unique_ptr<xAOD::CaloHitContainer>(new xAOD::CaloHitContainer()) );
    for (auto const& pair : hit_map)
    {
      hits->push_back(pair.second);
    }
  }

  MSG_INFO( "Writing new container event info...");
  {

    SG::ReadHandle<xAOD::EventInfoContainer> event(m_inputEventKey, ctx);
    if( !event.isValid() ){
      MSG_FATAL( "It's not possible to read the xAOD::EventInfoContainer from this Context" );
    }
    // get the old ones
    auto const_evt = (**event.ptr()).front();
    SG::WriteHandle<xAOD::EventInfoContainer> output_events(m_outputEventKey, ctx);
    output_events.record( std::unique_ptr<xAOD::EventInfoContainer>(new xAOD::EventInfoContainer()) );
    auto evt = new xAOD::EventInfo();
    evt->setAvgmu(nPileup);
    evt->setEventNumber( const_evt->eventNumber() );
    evt->setTotalEnergy( const_evt->totalEnergy()) ;
    output_events->push_back(evt);
  }
  
  delete collection_hits;
  delete collection_events;
  
  return StatusCode::SUCCESS;
 
}

//!=====================================================================

 StatusCode PileupMerge::fillHistograms( SG::EventContext &/*ctx*/ ) const
 {
    return StatusCode::SUCCESS;
 }

//!=====================================================================

template <class T>
void PileupMerge::InitBranch(TTree* fChain, std::string branch_name, T* param) const
{
  std::string bname = branch_name;
  if (fChain->GetAlias(bname.c_str()))
     bname = std::string(fChain->GetAlias(bname.c_str()));

  if (!fChain->FindBranch(bname.c_str()) ) {
    MSG_WARNING( "unknown branch " << bname );
    return;
  }
  fChain->SetBranchStatus(bname.c_str(), 1.);
  fChain->SetBranchAddress(bname.c_str(), param);
}



