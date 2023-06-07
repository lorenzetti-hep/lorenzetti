#include "CaloCell/CaloCellContainer.h"
#include "EventInfo/EventInfoContainer.h"
#include "TruthParticle/TruthParticleContainer.h"
#include "CaloCluster/CaloClusterContainer.h"
#include "CaloRings/CaloRingsContainer.h"
#include "CaloCell/CaloCellConverter.h"
#include "CaloCell/CaloDetDescriptorConverter.h"
#include "EventInfo/EventInfoConverter.h"
#include "TruthParticle/TruthParticleConverter.h"
#include "CaloCluster/CaloClusterConverter.h"
#include "CaloRings/CaloRingsConverter.h"
#include "RootStreamNTUPLEMaker.h"
#include "GaugiKernel/EDM.h"


using namespace SG;
using namespace Gaugi;



RootStreamNTUPLEMaker::RootStreamNTUPLEMaker( std::string name ) : 
  IMsgService(name),
  Algorithm()
{
  declareProperty( "InputFile"          , m_inputFile=""                    );
  declareProperty( "EventKey"           , m_eventKey="EventInfo"            );
  declareProperty( "TruthKey"           , m_truthKey="Particles"            );
  declareProperty( "CellsKey"           , m_cellsKey="Cells"                );
  declareProperty( "ClusterKey"         , m_clusterKey="Clusters"           );
  declareProperty( "RingerKey"          , m_ringerKey="Rings"               );
  declareProperty( "OutputLevel"        , m_outputLevel=1                   );
  declareProperty( "NtupleName"         , m_ntupleName="physics"            );
  declareProperty( "OutputNtupleName"   , m_outputNtupleName="events"       );
}

//!=====================================================================

RootStreamNTUPLEMaker::~RootStreamNTUPLEMaker()
{}

//!=====================================================================

StatusCode RootStreamNTUPLEMaker::initialize()
{
  CHECK_INIT();
  setMsgLevel(m_outputLevel);
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamNTUPLEMaker::finalize()
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamNTUPLEMaker::bookHistograms( EventContext &ctx ) const
{
  auto store = ctx.getStoreGateSvc();
  TFile *file = new TFile(m_inputFile.c_str(), "read");
  store->decorate( "events", file );

  
  store->cd();
  float eta = 0;
  float phi = 0;
  float e   = 0;
  float et  = 0;
  float deta         = 0;
  float dphi         = 0;
  float e0           = 0;
  float e1           = 0;
  float e2           = 0;
  float e3           = 0;
  float ehad1        = 0;
  float ehad2        = 0;
  float ehad3        = 0;
  float etot         = 0;
  float e233         = 0;
  float e237         = 0;
  float e277         = 0;
  float emaxs1       = 0;
  float emaxs2       = 0;
  float e2tsts1      = 0;
  float reta         = 0;
  float rphi         = 0;
  float rhad         = 0;
  float rhad1        = 0;
  float eratio       = 0;
  float f0           = 0;
  float f1           = 0;
  float f2           = 0;
  float f3           = 0;
  float weta2        = 0;
  std::vector<float>*rings        = nullptr;


  TTree *outputTree = new TTree(m_outputNtupleName.c_str(), "");
  outputTree->Branch( "cluster_eta",  &eta);
  outputTree->Branch( "cluster_phi",  &phi);
  outputTree->Branch( "cluster_e",    &e);
  outputTree->Branch( "cluster_et",   &et);
  outputTree->Branch("cluster_deta"   , &deta);
  outputTree->Branch("cluster_dphi"   , &dphi);
  outputTree->Branch("cluster_e0"     , &e0);
  outputTree->Branch("cluster_e1"     , &e1);
  outputTree->Branch("cluster_e2"     , &e2);
  outputTree->Branch("cluster_e3"     , &e3);
  outputTree->Branch("cluster_ehad1"  , &ehad1);
  outputTree->Branch("cluster_ehad2"  , &ehad2);
  outputTree->Branch("cluster_ehad3"  , &ehad3);
  outputTree->Branch("cluster_etot"   , &etot);
  outputTree->Branch("cluster_e233"   , &e233);
  outputTree->Branch("cluster_e237"   , &e237);
  outputTree->Branch("cluster_e277"   , &e277);
  outputTree->Branch("cluster_emaxs1" , &emaxs1);
  outputTree->Branch("cluster_emaxs2" , &emaxs2);
  outputTree->Branch("cluster_e2tsts1", &e2tsts1);
  outputTree->Branch("cluster_reta"   , &reta);
  outputTree->Branch("cluster_rphi"   , &rphi);
  outputTree->Branch("cluster_rhad"   , &rhad);
  outputTree->Branch("cluster_rhad1"  , &rhad1);
  outputTree->Branch("cluster_eratio" , &eratio);
  outputTree->Branch("cluster_f0"     , &f0);
  outputTree->Branch("cluster_f1"     , &f1);
  outputTree->Branch("cluster_f2"     , &f2);
  outputTree->Branch("cluster_f3"     , &f3);
  outputTree->Branch("cluster_weta2"  , &weta2);
  outputTree->Branch("rings"          , &rings);

  store->add(outputTree);
  return StatusCode::SUCCESS; 
}

//!=====================================================================

StatusCode RootStreamNTUPLEMaker::pre_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamNTUPLEMaker::execute( EventContext &/*ctx*/, const G4Step * /*step*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamNTUPLEMaker::execute( EventContext &ctx, int evt ) const
{
  return deserialize( evt, ctx );
}

//!=====================================================================

StatusCode RootStreamNTUPLEMaker::post_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamNTUPLEMaker::fillHistograms( EventContext &ctx ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamNTUPLEMaker::deserialize( int evt, EventContext &ctx ) const
{
  std::vector<xAOD::CaloDetDescriptor_t > *collection_descriptor = nullptr;
  std::vector<xAOD::CaloCell_t          > *collection_cells      = nullptr;
  std::vector<xAOD::EventInfo_t         > *collection_event      = nullptr;
  std::vector<xAOD::TruthParticle_t     > *collection_truth      = nullptr;
  std::vector<xAOD::CaloCluster_t       > *collection_cluster    = nullptr;
  std::vector<xAOD::CaloRings_t         > *collection_rings      = nullptr;

  MSG_DEBUG( "Link all branches..." );

  auto store = ctx.getStoreGateSvc();
  TFile *file = (TFile*)store->decorator("events");
  TTree *tree = (TTree*)file->Get(m_ntupleName.c_str());

  InitBranch( tree, ("EventInfoContainer_"         + m_eventKey).c_str() ,  &collection_event     );
  InitBranch( tree, ("TruthParticleContainer_"     + m_truthKey).c_str() ,  &collection_truth     );
  InitBranch( tree, ("CaloCellContainer_"          + m_cellsKey).c_str() ,  &collection_cells     );
  InitBranch( tree, ("CaloDetDescriptorContainer_" + m_cellsKey).c_str() ,  &collection_descriptor);
  InitBranch( tree, ("CaloClusterContainer_"       + m_clusterKey).c_str(), &collection_cluster);
  InitBranch( tree, ("CaloRingsContainer_"         + m_ringerKey).c_str(),  &collection_rings);

  tree->GetEntry( evt );

  store->cd();
  TTree *outputTree = store->tree(m_outputNtupleName.c_str());

  float eta = 0;
  float phi = 0;
  float e = 0;
  float et = 0;
  float deta         = 0;
  float dphi         = 0;
  float e0           = 0;
  float e1           = 0;
  float e2           = 0;
  float e3           = 0;
  float ehad1        = 0;
  float ehad2        = 0;
  float ehad3        = 0;
  float etot         = 0;
  float e233         = 0;
  float e237         = 0;
  float e277         = 0;
  float emaxs1       = 0;
  float emaxs2       = 0;
  float e2tsts1      = 0;
  float reta         = 0;
  float rphi         = 0;
  float rhad         = 0;
  float rhad1        = 0;
  float eratio       = 0;
  float f0           = 0;
  float f1           = 0;
  float f2           = 0;
  float f3           = 0;
  float weta2        = 0;
  std::vector<float> *rings = nullptr;
  
  InitBranch( outputTree, "cluster_eta" , &eta);
  InitBranch( outputTree, "cluster_phi" , &phi);
  InitBranch( outputTree, "cluster_e"   , &e);
  InitBranch( outputTree, "cluster_et"  , &et);
  InitBranch( outputTree,"cluster_deta"   , &deta);
  InitBranch( outputTree,"cluster_dphi"   , &dphi);
  InitBranch( outputTree,"cluster_e0"     , &e0);
  InitBranch( outputTree,"cluster_e1"     , &e1);
  InitBranch( outputTree,"cluster_e2"     , &e2);
  InitBranch( outputTree,"cluster_e3"     , &e3);
  InitBranch( outputTree,"cluster_ehad1"  , &ehad1);
  InitBranch( outputTree,"cluster_ehad2"  , &ehad2);
  InitBranch( outputTree,"cluster_ehad3"  , &ehad3);
  InitBranch( outputTree,"cluster_etot"   , &etot);
  InitBranch( outputTree,"cluster_e233"   , &e233);
  InitBranch( outputTree,"cluster_e237"   , &e237);
  InitBranch( outputTree,"cluster_e277"   , &e277);
  InitBranch( outputTree,"cluster_emaxs1" , &emaxs1);
  InitBranch( outputTree,"cluster_emaxs2" , &emaxs2);
  InitBranch( outputTree,"cluster_e2tsts1", &e2tsts1);
  InitBranch( outputTree,"cluster_reta"   , &reta);
  InitBranch( outputTree,"cluster_rphi"   , &rphi);
  InitBranch( outputTree,"cluster_rhad"   , &rhad);
  InitBranch( outputTree,"cluster_rhad1"  , &rhad1);
  InitBranch( outputTree,"cluster_eratio" , &eratio);
  InitBranch( outputTree,"cluster_f0"     , &f0);
  InitBranch( outputTree,"cluster_f1"     , &f1);
  InitBranch( outputTree,"cluster_f2"     , &f2);
  InitBranch( outputTree,"cluster_f3"     , &f3);
  InitBranch( outputTree,"cluster_weta2"  , &weta2);
  

  { //main loop (from cluster to truth)
  
    InitBranch(outputTree, "rings", &rings);
    SG::WriteHandle<xAOD::CaloRingsContainer> ringerContainer(m_ringerKey, ctx);
    ringerContainer.record( std::unique_ptr<xAOD::CaloRingsContainer>(new xAOD::CaloRingsContainer()));

    SG::WriteHandle<xAOD::CaloClusterContainer> clusterContainer(m_clusterKey, ctx);
    clusterContainer.record( std::unique_ptr<xAOD::CaloClusterContainer>(new xAOD::CaloClusterContainer()));
    int cluster_link = 0;
    for (auto &cluster_t: *collection_cluster){
      for (auto &ringer_t : *collection_rings ){
        if (ringer_t.cluster_link == cluster_link){
          MSG_DEBUG("Match between cluster and ringer");
          xAOD::CaloRings *ringer = nullptr;
          xAOD::CaloRingsConverter cnv;
          cnv.convert(ringer_t, ringer); // alloc memory
          for (auto ring : ringer->rings()) rings->push_back(ring);
          xAOD::CaloCluster *cluster = nullptr;
          xAOD::CaloClusterConverter clusterCnv;
          clusterCnv.convert(cluster_t, cluster);
          eta     = cluster->eta();
          phi     = cluster->phi();
          e       = cluster->e();
          et      = cluster->et();
          deta    = cluster->deltaEta();
          dphi    = cluster->deltaPhi();
          e0      = cluster->e0();
          e1      = cluster->e1();
          e2      = cluster->e2();
          e3      = cluster->e3();
          ehad1   = cluster->ehad1();
          ehad2   = cluster->ehad2();
          ehad3   = cluster->ehad3();
          etot    = cluster->etot();
          e233    = cluster->e233();
          e237    = cluster->e237();
          e277    = cluster->e277();
          emaxs1  = cluster->emaxs1();
          emaxs2  = cluster->emaxs2();
          e2tsts1 = cluster->e2tsts1();
          reta    = cluster->reta();
          rphi    = cluster->rphi();
          rhad    = cluster->rhad();
          rhad1   = cluster->rhad1();
          eratio  = cluster->eratio();
          f0      = cluster->f0();
          f1      = cluster->f1();
          f2      = cluster->f2();
          f3      = cluster->f3();
          weta2   = cluster->weta2();
          
        }
        break;
      }
      outputTree->Fill(); //each cluster should be 1 entry on ntulpe file
      cluster_link++;
    } //end loop of cluster
  } 

  eta   = 0;
  phi   = 0 ;
  e     = 0  ;
  et    = 0 ;
  deta         = 0;
  dphi         = 0;
  e0           = 0;
  e1           = 0;
  e2           = 0;
  e3           = 0;
  ehad1        = 0;
  ehad2        = 0;
  ehad3        = 0;
  etot         = 0;
  e233         = 0;
  e237         = 0;
  e277         = 0;
  emaxs1       = 0;
  emaxs2       = 0;
  e2tsts1      = 0;
  reta         = 0;
  rphi         = 0;
  rhad         = 0;
  rhad1        = 0;
  eratio       = 0;
  f0           = 0;
  f1           = 0;
  f2           = 0;
  f3           = 0;
  weta2        = 0;
  delete rings        ;

  delete collection_descriptor;
  delete collection_cells     ;
  delete collection_event     ;
  delete collection_truth     ;
  delete collection_cluster   ;

  return StatusCode::SUCCESS;
 
}

//!=====================================================================

template <class T>
void RootStreamNTUPLEMaker::InitBranch(TTree* fChain, std::string branch_name, T* param) const
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