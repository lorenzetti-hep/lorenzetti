#include "CaloCell/CaloCellContainer.h"
#include "EventInfo/EventInfoContainer.h"
#include "TruthParticle/TruthParticleContainer.h"
#include "CaloCluster/CaloClusterContainer.h"
#include "CaloRings/CaloRingsContainer.h"
#include "Egamma/ElectronContainer.h"

#include "CaloCell/CaloCellConverter.h"
#include "CaloCell/CaloDetDescriptorConverter.h"
#include "CaloCell/CaloDetDescriptorCollection.h"
#include "EventInfo/EventInfoConverter.h"
#include "TruthParticle/TruthParticleConverter.h"
#include "EventInfo/SeedConverter.h"
#include "CaloCluster/CaloClusterConverter.h"
#include "CaloRings/CaloRingsConverter.h"
#include "Egamma/ElectronConverter.h"
#include "RootStreamAODReader.h"
#include "GaugiKernel/EDM.h"


using namespace SG;
using namespace Gaugi;



RootStreamAODReader::RootStreamAODReader( std::string name ) : 
  IMsgService(name),
  Algorithm()
{


  declareProperty( "OutputEventKey"         , m_eventKey="EventInfo"            );
  declareProperty( "OutputSeedsKey"         , m_seedsKey="Seeds"                );
  declareProperty( "OutputTruthKey"         , m_truthKey="Particles"            );
  declareProperty( "OutputClusterKey"       , m_clusterKey="Clusters"           );
  declareProperty( "OutputRingerKey"        , m_ringerKey="Rings"               );
  declareProperty( "OutputElectronKey"      , m_electronKey="Electrons"         );
  declareProperty( "OutputLevel"            , m_outputLevel=1                   );
  declareProperty( "NtupleName"             , m_ntupleName="CollectionTree"     );
  declareProperty( "InputFile"              , m_inputFile=""                    );
}

//!=====================================================================

RootStreamAODReader::~RootStreamAODReader()
{}

//!=====================================================================

StatusCode RootStreamAODReader::initialize()
{
  CHECK_INIT();
  setMsgLevel(m_outputLevel);
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamAODReader::finalize()
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamAODReader::bookHistograms( EventContext &ctx ) const
{
  auto store = ctx.getStoreGateSvc();
  TFile *file = new TFile(m_inputFile.c_str(), "read");
  store->decorate( "events", file );
  return StatusCode::SUCCESS; 
}

//!=====================================================================

StatusCode RootStreamAODReader::pre_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamAODReader::execute( EventContext &/*ctx*/, const G4Step * /*step*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamAODReader::execute( EventContext &ctx, int evt ) const
{
  return deserialize( evt, ctx );
}

//!=====================================================================

StatusCode RootStreamAODReader::post_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamAODReader::fillHistograms( EventContext &ctx ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamAODReader::deserialize( int evt, EventContext &ctx ) const
{
  std::vector<xAOD::CaloDetDescriptor_t > *collection_descriptor = nullptr;
  std::vector<xAOD::CaloCell_t          > *collection_cells      = nullptr;
  std::vector<xAOD::EventInfo_t         > *collection_event      = nullptr;
  std::vector<xAOD::Seed_t              > *collection_seeds      = nullptr;
  std::vector<xAOD::TruthParticle_t     > *collection_truth      = nullptr;
  std::vector<xAOD::CaloRings_t         > *collection_rings      = nullptr;
  std::vector<xAOD::CaloCluster_t       > *collection_clus       = nullptr;
  std::vector<xAOD::Electron_t          > *collection_el         = nullptr;

  MSG_DEBUG( "Link all branches..." );

  auto store = ctx.getStoreGateSvc();
  TFile *file = (TFile*)store->decorator("events");
  TTree *tree = (TTree*)file->Get(m_ntupleName.c_str());

  InitBranch( tree, ("EventInfoContainer_"     + m_eventKey).c_str()       , &collection_event      );
  InitBranch( tree, ("SeedContainer_"          + m_seedsKey).c_str()       , &collection_seeds      );
  InitBranch( tree, ("TruthParticleContainer_" + m_truthKey).c_str()       , &collection_truth      );
  InitBranch( tree, ("CaloRingsContainer_"     + m_ringerKey).c_str()      , &collection_rings      );
  InitBranch( tree, ("CaloClusterContainer_"   + m_clusterKey).c_str()     , &collection_clus       );
  InitBranch( tree, ("ElectronContainer_"      + m_electronKey).c_str()    , &collection_el         );
  


  tree->GetEntry( evt );


  MSG_DEBUG("Deserialize TruthParticle...");
  
  { // deserialize EventInfo
    SG::WriteHandle<xAOD::TruthParticleContainer> container(m_truthKey, ctx);
    container.record( std::unique_ptr<xAOD::TruthParticleContainer>(new xAOD::TruthParticleContainer()));

    xAOD::TruthParticleConverter cnv;
    for( auto& par_t : *collection_truth)
    {
      xAOD::TruthParticle  *par=nullptr;
      cnv.convert(par_t, par);
      MSG_DEBUG( "Particle in eta = " << par->eta() << ", phi = " << par->phi());
      container->push_back(par);
    }
  }
  

  MSG_DEBUG("Deserialize EventInfo...");

  { // deserialize EventInfo

    SG::WriteHandle<xAOD::EventInfoContainer> container(m_eventKey, ctx);
    container.record( std::unique_ptr<xAOD::EventInfoContainer>(new xAOD::EventInfoContainer()));
    xAOD::EventInfo  *event=nullptr;
    xAOD::EventInfoConverter cnv;
    cnv.convert(  collection_event->at(0), event);
    MSG_DEBUG( "EventNumber = " << event->eventNumber() << ", Avgmu = " << event->avgmu());
    container->push_back(event);
  }
  

  MSG_DEBUG("Deserialize Seed...");
  std::map<int, xAOD::Seed*> seed_links;
  int seed_link=0;

  { // deserialize Seed
    SG::WriteHandle<xAOD::SeedContainer> container(m_seedsKey, ctx);
    container.record( std::unique_ptr<xAOD::SeedContainer>(new xAOD::SeedContainer()));

    xAOD::SeedConverter cnv;
    for( auto& seed_t : *collection_seeds)
    {
      xAOD::Seed  *seed=nullptr;
      cnv.convert(seed_t, seed);
      MSG_DEBUG( "Seed in eta = " << seed->eta() << ", phi = " << seed->phi());
      container->push_back(seed);
      seed_links[seed_link]=seed;
      seed_link++;
    }
  }



  MSG_DEBUG("Deserialize Clusters, Rings and Electrons...");
  
  { // deserialize cluster and rings
    SG::WriteHandle<xAOD::CaloClusterContainer> container_clus(m_clusterKey, ctx);
    container_clus.record( std::unique_ptr<xAOD::CaloClusterContainer>(new xAOD::CaloClusterContainer()));

    SG::WriteHandle<xAOD::CaloRingsContainer> container_rings(m_ringerKey, ctx);
    container_rings.record( std::unique_ptr<xAOD::CaloRingsContainer>(new xAOD::CaloRingsContainer()));

    SG::WriteHandle<xAOD::ElectronContainer> container_el(m_electronKey, ctx);
    container_el.record( std::unique_ptr<xAOD::ElectronContainer>(new xAOD::ElectronContainer()));


    xAOD::CaloClusterConverter clus_cnv;
    xAOD::CaloRingsConverter rings_cnv;
    xAOD::ElectronConverter el_cnv;
    
    std::map<int, xAOD::CaloCluster*> clus_links;
    int link=0;
    for( auto& clus_t : *collection_clus)
    {
      xAOD::CaloCluster  *clus=nullptr;
      clus_cnv.convert(clus_t, clus);
      clus->setSeed( seed_links[clus_t.seed_link] );
      container_clus->push_back(clus);
      clus_links[link] = clus;
      link++; 
    }

    for( auto& rings_t : *collection_rings)
    {
      xAOD::CaloRings  *rings=nullptr;
      rings_cnv.convert(rings_t, rings);
      rings->setCaloCluster( clus_links[rings_t.cluster_link] );
      container_rings->push_back(rings);
    }

    for( auto& el_t : *collection_el)
    {
      xAOD::Electron  *el=nullptr;
      el_cnv.convert(el_t, el);
      el->setCaloCluster( clus_links[el_t.cluster_link] );
      container_el->push_back(el);
    }
  }

  delete collection_descriptor;
  delete collection_seeds     ;
  delete collection_cells     ;
  delete collection_event     ;
  delete collection_truth     ;
  delete collection_rings     ;
  delete collection_clus      ;
  delete collection_el        ;


  return StatusCode::SUCCESS;
 
}

//!=====================================================================

template <class T>
void RootStreamAODReader::InitBranch(TTree* fChain, std::string branch_name, T* param) const
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

