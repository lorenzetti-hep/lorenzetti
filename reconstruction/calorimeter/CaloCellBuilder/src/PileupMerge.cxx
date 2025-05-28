

#include "PileupMerge.h"
#include "CaloHit/CaloHitConverter.h"
#include "EventInfo/EventInfoConverter.h"
#include "TTree.h"


using namespace SG;
using namespace Gaugi;



PileupMerge::PileupMerge( std::string name ) : 
  IMsgService(name),
  Algorithm(),
  m_rng(0)
{
  declareProperty( "InputHitsKey"       , m_inputHitsKey="Hits"                );
  declareProperty( "OutputHitsKey"      , m_outputHitsKey="Hits_Merged"        );
  declareProperty( "InputEventKey"      , m_inputEventKey="EventInfo"          );
  declareProperty( "OutputEventKey"     , m_outputEventKey="EventInfo_Merged"  );
  declareProperty( "OutputLevel"        , m_outputLevel=1                      );
  declareProperty( "NtupleName"         , m_ntupleName="CollectionTree"        );

  declareProperty( "LowPileupInputFile" , m_lowPileupInputFile=""              );
  declareProperty( "HighPileupInputFile", m_highPileupInputFile=""             );


  declareProperty( "PileupAvg"          , m_pileupAvg=0                        );
  declareProperty( "PileupSigma"        , m_pileupSigma=0                      );
  declareProperty( "TruncMu"            , m_trunc_mu = 1000                    );

  
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
  store->decorate( "unit_minbias" , new TFile(m_lowPileupInputFile.c_str(), "read") );
  store->decorate( "high_minbias", new TFile(m_highPileupInputFile.c_str(), "read") );
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

  float nWin, bcid_start, bcid_end = 0;

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

    if (!nWin){ // retrieve number of bunch crossings by hand from the first hit
      bcid_start=hit->bcid_start();
      bcid_end=hit->bcid_end();
      nWin = (bcid_end - bcid_start);
    }
    hit_map.insert( std::make_pair(hit->hash(), hit) );
  }
  

  MSG_DEBUG( "Link all branches..." );
  auto store = ctx.getStoreGateSvc();


  auto tree_unit_pileup = (TTree*)((TFile*)store->decorator("minbias_unit_pileup"))->Get(m_ntupleName.c_str());
  std::vector<xAOD::CaloHit_t> *collection_hits_unit_pileup=nullptr;
  InitBranch( tree_unit_pileup,  ("CaloHitContainer_"+m_inputHitsKey).c_str(), &collection_hits_unit_pileup );
  std::vector<xAOD::EventInfo_t> *collection_events_unit_pileup=nullptr;
  InitBranch( tree_unit_pileup,  ("EventInfoContainer_"+m_inputEventKey).c_str(), &collection_events_unit_pileup );

  auto tree_high_pileup = (TTree*)((TFile*)store->decorator("minbias_high_pileup"))->Get(m_ntupleName.c_str());
  std::vector<xAOD::CaloHit_t> *collection_hits_high_pileup=nullptr;
  InitBranch( tree_high_pileup,  ("CaloHitContainer_"+m_inputHitsKey).c_str(), &collection_hits_high_pileup );
  std::vector<xAOD::EventInfo_t> *collection_events_high_pileup=nullptr;
  InitBranch( tree_high_pileup,  ("EventInfoContainer_"+m_inputEventKey).c_str(), &collection_events_high_pileup );


  // NOTE: Get the number of pileup from the first event
  if (tree_high_pileup->GetEntry( 0 ) < 0){
    MSG_FATAL("Not possible to read this event. repeat...");
  }
  float nHighPileup = collection_events_high_pileup->at(0).totmu;



  std::vector<xAOD::CaloHit_t> *collection_hits=nullptr;
  std::vector<xAOD::EventInfo_t> *collection_events=nullptr;
  TTree *tree=nullptr;
  int pos(0);
  int nPileUpMean(0);
  float pileupAvg = m_rng.Gaus( m_pileupAvg, m_pileupSigma);


  for ( int bcid = bcid_start;  bcid <= bcid_end; ++bcid){

    int nPileup = poisson(pileupAvg);
    int kPileup=nPileup;

    nPileUpMean+=nPileup;

    while (kPileup>0){

      tree             = kPileup>nHighPileup? tree_high_pileup:tree_unit_pileup;
      collection_hits  = kPileup>nHighPileup? collection_hits_high_pileup : collection_hits_unit_pileup;
      collection_events= kPileup>nHighPileup? collection_events_high_pileup : collection_events_unit_pileup;
      
      int eventNumber = m_rng.Integer(tree->GetEntries()-1);

      if (tree->GetEntry( eventNumber ) < 0){
        MSG_FATAL("Not possible to read this event. repeat...");
      }

      kPileup -= collection_events->at(0).totmu;
      if (collection_hits->empty())
        MSG_WARNING("Collection hits is empty!");

      // merge the energy for each hit
      for (const auto& hit_t : *collection_hits )
      {
        if ( hit_map.count(hit_t.hash)){

          auto hit = hit_map.at(hit_t.hash);
          hit->edep( bcid, hit_t.edep.at(pos) ); // merge
        }
      }
    }
    MSG_INFO("Total of pileup " << nPileup << " remain... "<< kPileup)
    pos++;
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
    nPileUpMean/=nWin;

    SG::ReadHandle<xAOD::EventInfoContainer> event(m_inputEventKey, ctx);
    if( !event.isValid() ){
      MSG_FATAL( "It's not possible to read the xAOD::EventInfoContainer from this Context" );
    }
    // get the old ones
    auto const_evt = (**event.ptr()).front();
    SG::WriteHandle<xAOD::EventInfoContainer> output_events(m_outputEventKey, ctx);
    output_events.record( std::unique_ptr<xAOD::EventInfoContainer>(new xAOD::EventInfoContainer()) );
    auto evt = new xAOD::EventInfo();
    evt->setAvgmu(nPileUpMean);
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

//!=====================================================================

int PileupMerge::poisson(double nAvg) const
{
  // Random number.
  double rPoisson = m_rng.Uniform(0,1) * exp(nAvg);
  // Initialize.
  double rSum  = 0.;
  double rTerm = 1.;
  // Add to sum and check whether done.
  for (int i = 0; i < m_trunc_mu ; ) {
    rSum += rTerm;
    if (rSum > rPoisson) return i;
    // Evaluate next term.
    ++i;
    rTerm *= nAvg / i;
  }
  // Emergency return.
  return m_trunc_mu;
}

