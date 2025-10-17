

#include "PileupMerge.h"
#include "GaugiKernel/Timer.h"
#include "CaloHit/CaloHitConverter.h"
#include "EventInfo/EventInfoConverter.h"
#include "TTree.h"
#include "TChain.h"
#include <omp.h>
#include <stdexcept> 

using namespace SG;
using namespace Gaugi;



PileupMerge::PileupMerge( std::string name ) : 
  IMsgService(name),
  Algorithm(),
  m_rng(128)
{
  declareProperty( "InputHitsKey"        , m_inputHitsKey="Hits"                 );
  declareProperty( "OutputHitsKey"       , m_outputHitsKey="Hits_Merged"         );
  declareProperty( "InputEventKey"       , m_inputEventKey="EventInfo"           );
  declareProperty( "OutputEventKey"      , m_outputEventKey="EventInfo_Merged"   );
  declareProperty( "OutputLevel"         , m_outputLevel=1                       );
  declareProperty( "PileupAvg"           , m_pileupAvg=0                         );
  declareProperty( "PileupSigma"         , m_pileupSigma=0                       );
  declareProperty( "TruncMu"             , m_trunc_mu = 1000                     );
  declareProperty( "MergeMaxRetry"       , m_maxRetry=10                         );
  declareProperty( "Seed"                , m_seed=0                              );
  declareProperty( "BunchIdStart"        , m_bcid_start=-21                      );
  declareProperty( "BunchIdEnd"          , m_bcid_end=4                          );
  declareProperty( "NtupleName"          , m_ntupleName="CollectionTree"         );
  declareProperty( "LowPileupInputFiles" , m_lowPileupInputFiles={}              );
  declareProperty( "HighPileupInputFiles", m_highPileupInputFiles={}             );
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
  Read(ctx, m_lowPileupInputFiles , "low_minbias" );
  Read(ctx, m_highPileupInputFiles, "high_minbias");



  MSG_INFO( "Link all branches..." );
  auto store = ctx.getStoreGateSvc();


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


  Timer timer;
  timer.start();

  SG::ReadHandle<xAOD::CaloHitContainer> container(m_inputHitsKey, ctx);
  if( !container.isValid() )
  {
    MSG_FATAL("It's not possible to read the xAOD::CaloHitContainer from this Contaxt using this key " << m_inputHitsKey );
  }

  std::vector<xAOD::CaloHit*> vec_hits;
  unsigned retry=0;
  bool ok(false);
  float nPileupMean(0);

  // NOTE: For somereason i got many unzip errors when running a lot of get entry. In order
  // to avoid lose the event, we implement a retry mechanims with a try-catch to repeat the 
  // merge procedure in case of failure.
  while ( (retry < m_maxRetry) )
  {
    try{
      MSG_INFO("Processing retry "<< retry);
      MSG_INFO("Allocoate unconst hits...");
      allocate( container, vec_hits );
      MSG_INFO("Merging...")
      nPileupMean = merge(ctx, vec_hits);
      MSG_INFO("Pileup mean: "<< nPileupMean)
      break;
    }catch (const std::runtime_error& e) {
        std::cerr << "Caught an exception: " << e.what() << std::endl;
        retry++;
        MSG_INFO("Deallocating hits...");
        deallocate( vec_hits );
    }
  }

  MSG_INFO("Number of retries: "<<retry);
  
  bool save = retry<m_maxRetry?true:false;

  if (save){

    MSG_INFO( "Writing new container hits...");
    {
      SG::WriteHandle<xAOD::CaloHitContainer> hits(m_outputHitsKey, ctx);
      hits.record( std::unique_ptr<xAOD::CaloHitContainer>(new xAOD::CaloHitContainer()) );
      for (auto hit : vec_hits)
      {
        hits->push_back(hit);
      }
    }

    MSG_INFO( "Writing new container event info...");
    {
      MSG_INFO("Avgmu: " << nPileupMean);
      SG::ReadHandle<xAOD::EventInfoContainer> event(m_inputEventKey, ctx);
      if( !event.isValid() ){
        MSG_FATAL( "It's not possible to read the xAOD::EventInfoContainer from this Context" );
      }
      // get the old ones
      auto const_evt = (**event.ptr()).front();
      SG::WriteHandle<xAOD::EventInfoContainer> output_events(m_outputEventKey, ctx);
      output_events.record( std::unique_ptr<xAOD::EventInfoContainer>(new xAOD::EventInfoContainer()) );
      auto evt = new xAOD::EventInfo();
      evt->setAvgmu(nPileupMean);
      evt->setEventNumber( const_evt->eventNumber() );
      evt->setTotalEnergy( const_evt->totalEnergy()) ;
      output_events->push_back(evt);
    }
  }else{
    MSG_WARNING("Its not possible to save this event because of a merge problem!");
  }

  timer.stop();
  MSG_INFO("Event merged in " << timer.resume() << " seconds.");  
  return StatusCode::SUCCESS;
}

//!=====================================================================


float PileupMerge::merge( EventContext &ctx, std::vector<xAOD::CaloHit*> &vec_hits ) const{

  MSG_INFO( "Link all branches..." );
  auto store = ctx.getStoreGateSvc();

  auto tree_low_pileup = ((TChain*)store->decorator("low_minbias"));
  tree_low_pileup->SetBranchStatus("*",0);
  tree_low_pileup->SetBranchStatus(("CaloHitContainer_"+m_inputHitsKey+".edep").c_str(), 1);
  //tree_low_pileup->SetBranchStatus(("CaloHitContainer_"+m_inputHitsKey+".tof").c_str(), 1);
  tree_low_pileup->SetBranchStatus(("CaloHitContainer_"+m_inputHitsKey+".bcid_start").c_str(), 1);
  tree_low_pileup->SetBranchStatus(("CaloHitContainer_"+m_inputHitsKey+".bcid_end").c_str(), 1);
  tree_low_pileup->SetBranchStatus(("CaloHitContainer_"+m_inputHitsKey+".hash").c_str(), 1);
  tree_low_pileup->SetBranchStatus(("EventInfoContainer_"+m_inputEventKey+".avgmu").c_str(), 1);
  



  std::vector<xAOD::CaloHit_t> *collection_hits_low_pileup=nullptr;
  InitBranch( tree_low_pileup,  ("CaloHitContainer_"+m_inputHitsKey).c_str(), &collection_hits_low_pileup );
  std::vector<xAOD::EventInfo_t> *collection_events_low_pileup=nullptr;
  InitBranch( tree_low_pileup,  ("EventInfoContainer_"+m_inputEventKey).c_str(), &collection_events_low_pileup );

  auto tree_high_pileup = ((TChain*)store->decorator("high_minbias"));
  tree_high_pileup->SetBranchStatus("*",0);
  tree_high_pileup->SetBranchStatus(("CaloHitContainer_"+m_inputHitsKey+".edep").c_str(), 1);
  //tree_high_pileup->SetBranchStatus(("CaloHitContainer_"+m_inputHitsKey+".tof").c_str(), 1);
  tree_high_pileup->SetBranchStatus(("CaloHitContainer_"+m_inputHitsKey+".bcid_start").c_str(), 1);
  tree_high_pileup->SetBranchStatus(("CaloHitContainer_"+m_inputHitsKey+".bcid_end").c_str(), 1);
  tree_high_pileup->SetBranchStatus(("CaloHitContainer_"+m_inputHitsKey+".hash").c_str(), 1);
  tree_high_pileup->SetBranchStatus(("EventInfoContainer_"+m_inputEventKey+".avgmu").c_str(), 1);





  std::vector<xAOD::CaloHit_t> *collection_hits_high_pileup=nullptr;
  InitBranch( tree_high_pileup,  ("CaloHitContainer_"+m_inputHitsKey).c_str(), &collection_hits_high_pileup );
  std::vector<xAOD::EventInfo_t> *collection_events_high_pileup=nullptr;
  InitBranch( tree_high_pileup,  ("EventInfoContainer_"+m_inputEventKey).c_str(), &collection_events_high_pileup );

  if (tree_high_pileup->GetEntry( 0 ) < 0){
    MSG_FATAL("Not possible to read this event. repeat...");
  }

  float nHighPileup = collection_events_high_pileup->at(0).avgmu;
  int nWin = m_bcid_end - m_bcid_start;
  std::map<unsigned long int, int > hit_map;

  for (unsigned int idx=0; idx<(*collection_hits_high_pileup).size(); idx++ )
  {
    auto hit_t = (*collection_hits_high_pileup).at(idx);
    hit_map.insert( std::make_pair(hit_t.hash, idx) );
  }


  std::vector<xAOD::CaloHit_t>   *collection_hits=nullptr;
  std::vector<xAOD::EventInfo_t> *collection_events=nullptr;
  TTree *tree=nullptr;

  int nPileUpMean(0);
  int pileupAvg = (int)m_rng.Gaus( m_pileupAvg, m_pileupSigma);

  MSG_INFO("Merging with Pileup Avg: "<< pileupAvg);

  int eventNumber=-1;

  for ( int bcid = m_bcid_start;  bcid <= m_bcid_end; ++bcid){

    int nPileup = poisson(pileupAvg);
    int kPileup=nPileup;
    nPileUpMean+=nPileup;

    while (kPileup>0)
    {
      tree                 = kPileup>nHighPileup? tree_high_pileup                 : tree_low_pileup;
      collection_hits      = kPileup>nHighPileup? collection_hits_high_pileup      : collection_hits_low_pileup;
      collection_events    = kPileup>nHighPileup? collection_events_high_pileup    : collection_events_low_pileup;
  


      if (eventNumber > (tree->GetEntries()-1)){
        eventNumber=-1;
      }

      if (eventNumber<0){
        eventNumber = m_rng.Integer(tree->GetEntries()-1);
      }
      
      MSG_DEBUG("Reading event " << eventNumber);
   

      if (tree->GetEntry( eventNumber ) <= 0){
        MSG_FATAL("Not possible to read this event. repeat...");
      }

      if (collection_events->empty()){
        MSG_FATAL("Collection hits is empty!");
      }
      if (collection_hits->empty()){
        MSG_FATAL("Collection hits is empty!");
      }
      eventNumber++;
      kPileup -= collection_events->at(0).avgmu;

      for (auto hit_main : vec_hits)
      {
        if(hit_map.count(hit_main->hash()))
        {
          int idx = hit_map[hit_main->hash()];
          auto hit_t = (*collection_hits).at(idx);
          xAOD::CaloHit *hit = nullptr;
          xAOD::CaloHitConverter cnv;
          cnv.convert(hit_t, hit);
          hit_main->edep( bcid, hit->edep(bcid) ); 
          delete hit;
        }
      }
      MSG_DEBUG( "Merging a total of " << nPileup << " pileup into bunch-crossing " << bcid << ". Current pileup is " <<(nPileup - kPileup));  
    }// while
  }

  delete collection_hits;
  delete collection_events;
  return nPileUpMean/nWin;
}

//!=====================================================================

void PileupMerge::allocate( SG::ReadHandle<xAOD::CaloHitContainer> &container , std::vector<xAOD::CaloHit*> &vec_hits ) const{

  MSG_INFO( "Convert hits to hash map with size " << container.ptr()->size() <<"...");
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
    vec_hits.push_back(hit);
  }
  
}

void PileupMerge::deallocate( std::vector<xAOD::CaloHit*> &vec_hits ) const{
  for (auto hit : vec_hits)
  {
    delete hit;
  }  
  vec_hits.clear();
}



//!=====================================================================

 StatusCode PileupMerge::fillHistograms( SG::EventContext &/*ctx*/ ) const
 {
    return StatusCode::SUCCESS;
 }

//!=====================================================================

template <class T>
TBranch* PileupMerge::InitBranch(TTree* fChain, std::string branch_name, T* param) const
{
  std::string bname = branch_name;
  if (fChain->GetAlias(bname.c_str()))
     bname = std::string(fChain->GetAlias(bname.c_str()));

  if (!fChain->FindBranch(bname.c_str()) ) {
    MSG_FATAL( "unknown branch " << bname );
  }
  fChain->SetBranchStatus(bname.c_str(), 1.);
  fChain->SetBranchAddress(bname.c_str(), param);
  MSG_INFO("branch aki = "<<fChain->GetBranch(bname.c_str()));

  return fChain->GetBranch(bname.c_str());  
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

//!=====================================================================


void PileupMerge::Read( EventContext &ctx, const std::vector<std::string> &paths, std::string name ) const
{
  auto store = ctx.getStoreGateSvc();
  TChain *file = new TChain();
  for (auto path : paths){
    //MSG_INFO("Loading " << path << "...");
    file->Add((path+"/"+m_ntupleName).c_str());
  }
  store->decorate( name , file );
}
