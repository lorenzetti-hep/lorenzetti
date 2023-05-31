#include "CaloCell/CaloCellContainer.h"
#include "EventInfo/EventInfoContainer.h"
#include "TruthParticle/TruthParticleContainer.h"
#include "CaloCluster/CaloClusterContainer.h"
#include "CaloCell/CaloCellConverter.h"
#include "CaloCell/CaloDetDescriptorConverter.h"
#include "EventInfo/EventInfoConverter.h"
#include "TruthParticle/TruthParticleConverter.h"
#include "CaloCluster/CaloClusterConverter.h"
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
  std::vector<float> *eta = nullptr;
  std::vector<float> *phi = nullptr;
  std::vector<float> *e   = nullptr;
  std::vector<float> *et  = nullptr;
  std::vector<float>*deta         = nullptr;
  std::vector<float>*dphi         = nullptr;
  std::vector<float>*e0           = nullptr;
  std::vector<float>*e1           = nullptr;
  std::vector<float>*e2           = nullptr;
  std::vector<float>*e3           = nullptr;
  std::vector<float>*ehad1        = nullptr;
  std::vector<float>*ehad2        = nullptr;
  std::vector<float>*ehad3        = nullptr;
  std::vector<float>*etot         = nullptr;
  std::vector<float>*e233         = nullptr;
  std::vector<float>*e237         = nullptr;
  std::vector<float>*e277         = nullptr;
  std::vector<float>*emaxs1       = nullptr;
  std::vector<float>*emaxs2       = nullptr;
  std::vector<float>*e2tsts1      = nullptr;
  std::vector<float>*reta         = nullptr;
  std::vector<float>*rphi         = nullptr;
  std::vector<float>*rhad         = nullptr;
  std::vector<float>*rhad1        = nullptr;
  std::vector<float>*eratio       = nullptr;
  std::vector<float>*f0           = nullptr;
  std::vector<float>*f1           = nullptr;
  std::vector<float>*f2           = nullptr;
  std::vector<float>*f3           = nullptr;
  std::vector<float>*weta2        = nullptr;


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

  MSG_DEBUG( "Link all branches..." );

  auto store = ctx.getStoreGateSvc();
  TFile *file = (TFile*)store->decorator("events");
  TTree *tree = (TTree*)file->Get(m_ntupleName.c_str());

  InitBranch( tree, ("EventInfoContainer_"         + m_eventKey).c_str() ,  &collection_event     );
  InitBranch( tree, ("TruthParticleContainer_"     + m_truthKey).c_str() ,  &collection_truth     );
  InitBranch( tree, ("CaloCellContainer_"          + m_cellsKey).c_str() ,  &collection_cells     );
  InitBranch( tree, ("CaloDetDescriptorContainer_" + m_cellsKey).c_str() ,  &collection_descriptor);
  InitBranch( tree, ("CaloClusterContainer_"       + m_clusterKey).c_str(), &collection_cluster);

  tree->GetEntry( evt );

  
  { // deserialize EventInfo
    SG::WriteHandle<xAOD::TruthParticleContainer> container(m_truthKey, ctx);
    container.record( std::unique_ptr<xAOD::TruthParticleContainer>(new xAOD::TruthParticleContainer()));

    xAOD::TruthParticleConverter cnv;
    for( auto& par_t : *collection_truth)
    {
      xAOD::TruthParticle  *par=nullptr;
      cnv.convert(par_t, par);
      MSG_INFO( "Particle seeded in eta = " << par->eta() << ", phi = " << par->phi());
      container->push_back(par);
    }
  }



  { // deserialize EventInfo

    SG::WriteHandle<xAOD::EventInfoContainer> container(m_eventKey, ctx);
    container.record( std::unique_ptr<xAOD::EventInfoContainer>(new xAOD::EventInfoContainer()));
    xAOD::EventInfo  *event=nullptr;
    xAOD::EventInfoConverter cnv;
    cnv.convert(  collection_event->at(0), event);
    MSG_INFO( "EventNumber = " << event->eventNumber() << ", Avgmu = " << event->avgmu());
    container->push_back(event);
  }
  

  {
    
    std::map<int, xAOD::CaloDetDescriptor*> descriptor_links;

    int link=0;
    for (auto &descriptor_t : *collection_descriptor )
    {
      xAOD::CaloDetDescriptor *descriptor = nullptr;
      xAOD::CaloDetDescriptorConverter cnv;
      cnv.convert(descriptor_t, descriptor); // alloc memory
      descriptor_links[link] = descriptor;
      link++;
    }

    SG::WriteHandle<xAOD::CaloCellContainer> container(m_cellsKey, ctx);
    container.record( std::unique_ptr<xAOD::CaloCellContainer>(new xAOD::CaloCellContainer()));

    for( auto &cell_t : *collection_cells )
    {
      xAOD::CaloCell *cell = nullptr;
      xAOD::CaloCellConverter cnv;
      cnv.convert(cell_t, cell); // alloc memory
      cell->setDescriptor( descriptor_links[cell_t.descriptor_link] );
      container->push_back(cell);
    }

    
  }

  store->cd();
  TTree *outputTree = store->tree(m_outputNtupleName.c_str());

  std::vector<float> *eta = nullptr;
  std::vector<float> *phi = nullptr;
  std::vector<float> *e = nullptr;
  std::vector<float> *et = nullptr;
  std::vector<float>*deta         = nullptr;
  std::vector<float>*dphi         = nullptr;
  std::vector<float>*e0           = nullptr;
  std::vector<float>*e1           = nullptr;
  std::vector<float>*e2           = nullptr;
  std::vector<float>*e3           = nullptr;
  std::vector<float>*ehad1        = nullptr;
  std::vector<float>*ehad2        = nullptr;
  std::vector<float>*ehad3        = nullptr;
  std::vector<float>*etot         = nullptr;
  std::vector<float>*e233         = nullptr;
  std::vector<float>*e237         = nullptr;
  std::vector<float>*e277         = nullptr;
  std::vector<float>*emaxs1       = nullptr;
  std::vector<float>*emaxs2       = nullptr;
  std::vector<float>*e2tsts1      = nullptr;
  std::vector<float>*reta         = nullptr;
  std::vector<float>*rphi         = nullptr;
  std::vector<float>*rhad         = nullptr;
  std::vector<float>*rhad1        = nullptr;
  std::vector<float>*eratio       = nullptr;
  std::vector<float>*f0           = nullptr;
  std::vector<float>*f1           = nullptr;
  std::vector<float>*f2           = nullptr;
  std::vector<float>*f3           = nullptr;
  std::vector<float>*weta2        = nullptr;
  
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


  
  {

    SG::WriteHandle<xAOD::CaloClusterContainer> container(m_clusterKey, ctx);
    container.record( std::unique_ptr<xAOD::CaloClusterContainer>(new xAOD::CaloClusterContainer()));



    for( auto &cluster_t : *collection_cluster )
    {
      xAOD::CaloCluster *cluster = nullptr;
      xAOD::CaloClusterConverter cnv;
      cnv.convert(cluster_t, cluster); // alloc memory
      // cell->setDescriptor( descriptor_links[cell_t.descriptor_link] );
      MSG_INFO( "Cluster seeded in eta = " << cluster->eta() << ", phi = " << cluster->phi());
      MSG_INFO( "Cluster transverse energy = " << cluster->et());
      eta->push_back(cluster->eta());
      phi->push_back(cluster->phi());
      e->push_back(cluster->e());
      et->push_back(cluster->et());
      deta   ->push_back( cluster->deltaEta());
      dphi   ->push_back( cluster->deltaPhi());
      e0     ->push_back( cluster->e0());
      e1     ->push_back( cluster->e1());
      e2     ->push_back( cluster->e2());
      e3     ->push_back( cluster->e3());
      ehad1  ->push_back( cluster->ehad1());
      ehad2  ->push_back( cluster->ehad2());
      ehad3  ->push_back( cluster->ehad3());
      etot   ->push_back( cluster->etot());
      e233   ->push_back( cluster->e233());
      e237   ->push_back( cluster->e237());
      e277   ->push_back( cluster->e277());
      emaxs1 ->push_back( cluster->emaxs1());
      emaxs2 ->push_back( cluster->emaxs2());
      e2tsts1->push_back( cluster->e2tsts1());
      reta   ->push_back( cluster->reta());
      rphi   ->push_back( cluster->rphi());
      rhad   ->push_back( cluster->rhad());
      rhad1  ->push_back( cluster->rhad1());
      eratio ->push_back( cluster->eratio());
      f0     ->push_back( cluster->f0());
      f1     ->push_back( cluster->f1());
      f2     ->push_back( cluster->f2());
      f3     ->push_back( cluster->f3());
      weta2  ->push_back( cluster->weta2());
      container->push_back(cluster);
    }
 
  }

  outputTree->Fill();

  delete eta ;
  delete phi ;
  delete e   ;
  delete et  ;
  delete deta         ;
  delete dphi         ;
  delete e0           ;
  delete e1           ;
  delete e2           ;
  delete e3           ;
  delete ehad1        ;
  delete ehad2        ;
  delete ehad3        ;
  delete etot         ;
  delete e233         ;
  delete e237         ;
  delete e277         ;
  delete emaxs1       ;
  delete emaxs2       ;
  delete e2tsts1      ;
  delete reta         ;
  delete rphi         ;
  delete rhad         ;
  delete rhad1        ;
  delete eratio       ;
  delete f0           ;
  delete f1           ;
  delete f2           ;
  delete f3           ;
  delete weta2        ;

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

