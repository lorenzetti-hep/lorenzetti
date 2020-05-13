
#include "CaloCell/CaloCellContainer.h"
#include "CaloCluster/CaloClusterContainer.h"
#include "CaloRings/CaloRingsContainer.h"
#include "EventInfo/EventInfoContainer.h"
#include "TruthParticle/TruthParticleContainer.h"
#include "TTree.h"
#include "RawNtupleMaker.h"
#include "TMath.h"

using namespace SG;
using namespace CaloSampling;
using namespace Gaugi;



RawNtupleMaker::RawNtupleMaker( std::string name ) : 
  IMsgService(name),
  Algorithm()
{
  declareProperty( "EventKey"       , m_eventKey="EventInfo"            );
  declareProperty( "CellsKey"       , m_cellsKey="Cells"                );
  declareProperty( "OutputLevel"    , m_outputLevel=1                   );
  declareProperty( "NtupleName"     , m_ntupleName="raw"                );
}



RawNtupleMaker::~RawNtupleMaker()
{;}


StatusCode RawNtupleMaker::initialize()
{
  setMsgLevel(m_outputLevel);
  return StatusCode::SUCCESS;
}


StatusCode RawNtupleMaker::bookHistograms( StoreGate &store ) const
{
  // Create all local variables since this must be a const method
  int eventNumber         = -1;
  float avgmu             = -1;
  float seed_eta          = -1;
  float seed_phi          = -1;
  
  std::vector<float> cell_et          ;
  std::vector<float> cell_eta         ;
  std::vector<float> cell_phi         ;
  std::vector<float> cell_deta        ;
  std::vector<float> cell_dphi        ;
  std::vector<float> cell_energy      ;
  std::vector<int>   mc_cell_sampling ; 
  std::vector<int>   cell_sampling    ;
  std::string        cell_hash        ;
  std::vector<float> cell_samples     ;
 
  store.cd();
  TTree *tree = new TTree(m_ntupleName.c_str(), "");
  
  tree->Branch(  "EventNumber"        , &eventNumber        );
  tree->Branch(  "avgmu"              , &avgmu              );
  tree->Branch(  "seed_eta"           , &seed_eta           );
  tree->Branch(  "seed_phi"           , &seed_phi           );
  
  tree->Branch(  "cell_et"            , &cell_et            );
  tree->Branch(  "cell_eta"           , &cell_eta           );
  tree->Branch(  "cell_phi"           , &cell_phi           );
  tree->Branch(  "cell_deta"          , &cell_deta          );
  tree->Branch(  "cell_dphi"          , &cell_dphi          );
  tree->Branch(  "cell_energy"        , &cell_energy        );
  tree->Branch(  "cell_sampling"      , &cell_sampling      );
//  tree->Branch(  cell_hash.c_str()    , &cell_samples       );
  tree->Branch(  "cell_samples"       , &cell_samples       );
 

  store.add( tree );
  
  return StatusCode::SUCCESS;
}


StatusCode RawNtupleMaker::pre_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}


StatusCode RawNtupleMaker::execute( EventContext &/*ctx*/, const G4Step * /*step*/ ) const
{
  return StatusCode::SUCCESS;
}


StatusCode RawNtupleMaker::finalize()
{
  return StatusCode::SUCCESS;
}


StatusCode RawNtupleMaker::post_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}





StatusCode RawNtupleMaker::fillHistograms( EventContext &ctx , StoreGate &store ) const
{
  store.cd();
  TTree *tree = store.tree(m_ntupleName);

  Fill( ctx, tree );

  return StatusCode::SUCCESS;
}


template <class T>
void RawNtupleMaker::InitBranch(TTree* fChain, std::string branch_name, T* param) const
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





void RawNtupleMaker::Fill( EventContext &ctx , TTree *tree  ) const
{
  // Create all local variables since this must be a const method
  int   eventNumber    ;
  float avgmu          ;
  float seed_eta       ;
  float seed_phi       ;
  
  std::vector<float> *cell_et          = nullptr;
  std::vector<float> *cell_eta         = nullptr;
  std::vector<float> *cell_phi         = nullptr;
  std::vector<float> *cell_deta        = nullptr;
  std::vector<float> *cell_dphi        = nullptr;
  std::vector<float> *cell_energy      = nullptr;
  std::vector<int>   *cell_sampling    = nullptr;
  std::string        cell_hash                  ;
  std::vector<float> *cell_samples     = nullptr;

  InitBranch( tree,  "EventNumber"        , &eventNumber        );
  InitBranch( tree,  "avgmu"              , &avgmu              );
  InitBranch( tree,  "seed_eta"           , &seed_eta           );
  InitBranch( tree,  "seed_phi"           , &seed_phi           );
  InitBranch( tree,  "cell_et"            , &cell_et            );
  InitBranch( tree,  "cell_eta"           , &cell_eta           );
  InitBranch( tree,  "cell_phi"           , &cell_phi           );
  InitBranch( tree,  "cell_deta"          , &cell_deta          );
  InitBranch( tree,  "cell_dphi"          , &cell_dphi          );
  InitBranch( tree,  "cell_energy"        , &cell_energy        );
  InitBranch( tree,  "cell_sampling"      , &cell_sampling      );
  //  InitBranch( tree,  cell_hash.c_str()    , &cell_samples       );
  InitBranch( tree,  "cell_samples"       , &cell_samples       );
  
  MSG_DEBUG( "Link all branches..." );

  seed_eta            = 0;
  seed_phi            = 0;
  
  cell_et->clear()         ;
  cell_eta->clear()        ;
  cell_phi->clear()        ;
  cell_deta->clear()       ;
  cell_dphi->clear()       ;
  cell_energy->clear()     ;
  cell_sampling->clear()   ;
  cell_samples->clear()    ;
  cell_hash.clear()        ;




  // Event info
  SG::ReadHandle<xAOD::EventInfoContainer> event(m_eventKey, ctx);

  if( !event.isValid() ){
    MSG_FATAL( "It's not possible to read the xAOD::EventInfoContainer from this Context" );
  }

  SG::ReadHandle<xAOD::CaloCellContainer> container(m_cellsKey, ctx);

  if( !container.isValid() ){
    MSG_WARNING( "It's not possible to read the xAOD::CaloCellContainer from this Context using this key: " << m_cellsKey );
    return;
  }



  avgmu = (**event.ptr()).front()->avgmu();
  eventNumber = (**event.ptr()).front()->eventNumber();


  //auto seeds = (**event.ptr()).front()->allSeeds();
  
  
  for ( const auto cell : **container.ptr() ){

    auto raw = cell->parent();
    cell_hash = raw->hash();

    // Make something here...

    cell_et->push_back( cell->et() );
    cell_eta->push_back( cell->eta() );
    cell_phi->push_back( cell->phi() );
    cell_deta->push_back( cell->deltaEta() );
    cell_dphi->push_back( cell->deltaPhi() );
    cell_energy->push_back( cell->energy() );
    cell_sampling->push_back( (int)cell->sampling() );
    for (auto it = raw->pulse().begin(); it!=raw->pulse().end(); it++) cell_samples->push_back(*it);


  }// Loop over all cells

  tree->Fill();
}


