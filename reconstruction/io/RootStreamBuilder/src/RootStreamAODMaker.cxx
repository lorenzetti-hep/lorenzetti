#include "CaloCell/CaloCellContainer.h"
#include "CaloCluster/CaloClusterContainer.h"
#include "CaloRings/CaloRingsContainer.h"
#include "EventInfo/EventInfoContainer.h"
#include "TruthParticle/TruthParticleContainer.h"
#include "Egamma/ElectronContainer.h"

#include "CaloCell/CaloCellConverter.h"
#include "CaloCell/CaloDetDescriptorConverter.h"
#include "CaloCluster/CaloClusterConverter.h"
#include "CaloRings/CaloRingsConverter.h"
#include "EventInfo/EventInfoConverter.h"
#include "EventInfo/SeedConverter.h"
#include "TruthParticle/TruthParticleConverter.h"
#include "Egamma/ElectronConverter.h"
#include "TTree.h"
#include "RootStreamAODMaker.h"
#include "GaugiKernel/EDM.h"


using namespace SG;
using namespace Gaugi;



RootStreamAODMaker::RootStreamAODMaker( std::string name ) : 
  IMsgService(name),
  Algorithm()
{
  declareProperty( "InputEventKey"           , m_inputEventKey="EventInfo"            );
  declareProperty( "InputSeedsKey"           , m_inputSeedsKey="Seeds"                );
  declareProperty( "InputTruthKey"           , m_inputTruthKey="Particles"            );
  declareProperty( "InputCellsKey"           , m_inputCellsKey="Cells"                );
  declareProperty( "InputClusterKey"         , m_inputClusterKey="Clusters"           );
  declareProperty( "InputRingerKey"          , m_inputRingerKey="Rings"               );
  declareProperty( "InputElectronKey"        , m_inputElectronKey="Electrons"         );

  declareProperty( "OutputEventKey"          , m_outputEventKey="EventInfo"           );
  declareProperty( "OutputSeedsKey"          , m_outputSeedsKey="Seeds"               );
  declareProperty( "OutputTruthKey"          , m_outputTruthKey="Particles"           );
  declareProperty( "OutputCellsKey"          , m_outputCellsKey="Cells"               );
  declareProperty( "OutputClusterKey"        , m_outputClusterKey="Clusters"          );
  declareProperty( "OutputRingerKey"         , m_outputRingerKey="Rings"              );
  declareProperty( "OutputElectronKey"       , m_outputElectronKey="Electrons"        );

  declareProperty( "OutputLevel"             , m_outputLevel=1                        );
  declareProperty( "NtupleName"              , m_ntupleName="physics"                 );
  declareProperty( "DumpCells"               , m_dumpCells=false                      );
}

//!=====================================================================

RootStreamAODMaker::~RootStreamAODMaker()
{;}

//!=====================================================================

StatusCode RootStreamAODMaker::initialize()
{
  CHECK_INIT();
  setMsgLevel(m_outputLevel);
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamAODMaker::bookHistograms( SG::EventContext &ctx ) const
{
  auto store = ctx.getStoreGateSvc();

  std::vector<xAOD::CaloCell_t            > container_cells;
  std::vector<xAOD::CaloDetDescriptor_t   > container_descriptor;
  std::vector<xAOD::CaloCluster_t         > container_clus;
  std::vector<xAOD::CaloRings_t           > container_rings;
  std::vector<xAOD::EventInfo_t           > container_event;
  std::vector<xAOD::Seed_t           > container_seeds;
  std::vector<xAOD::TruthParticle_t       > container_truth;
  std::vector<xAOD::Electron_t            > container_electron;

  store->cd();
  TTree *tree = new TTree(m_ntupleName.c_str(), "");

  tree->Branch( ("EventInfoContainer_"     + m_outputEventKey).c_str()       , &container_event      );
  tree->Branch( ("SeedContainer_"     + m_outputSeedsKey).c_str()       , &container_seeds      );
  tree->Branch( ("TruthParticleContainer_" + m_outputTruthKey).c_str()       , &container_truth      );
  tree->Branch( ("CaloRingsContainer_"     + m_outputRingerKey).c_str()      , &container_rings      );
  tree->Branch( ("CaloClusterContainer_"   + m_outputClusterKey).c_str()     , &container_clus       );
  tree->Branch( ("ElectronContainer_"      + m_outputElectronKey).c_str()     , &container_electron  );
  if(m_dumpCells){
    tree->Branch(  ("CaloCellContainer_"          + m_outputCellsKey).c_str()   , &container_cells      );
    tree->Branch(  ("CaloDetDescriptorContainer_" + m_outputCellsKey).c_str()   , &container_descriptor );
  }


  store->add( tree );
  
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamAODMaker::pre_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamAODMaker::execute( EventContext &/*ctx*/, const G4Step * /*step*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamAODMaker::execute( EventContext &/*ctx*/, int /*evt*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamAODMaker::post_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamAODMaker::fillHistograms( EventContext &ctx ) const
{
  return serialize(ctx);
}

//!=====================================================================


template <class T>
void RootStreamAODMaker::InitBranch(TTree* fChain, std::string branch_name, T* param) const
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

StatusCode RootStreamAODMaker::finalize()
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamAODMaker::serialize( EventContext &ctx ) const
{
  

  auto store = ctx.getStoreGateSvc();

  store->cd();
  TTree *tree = store->tree(m_ntupleName);
 
  std::vector<xAOD::CaloDetDescriptor_t > *container_descriptor = nullptr;
  std::vector<xAOD::CaloCell_t          > *container_cells      = nullptr;
  std::vector<xAOD::CaloCluster_t       > *container_clus       = nullptr;
  std::vector<xAOD::CaloRings_t         > *container_rings      = nullptr;
  std::vector<xAOD::EventInfo_t         > *container_event      = nullptr;
  std::vector<xAOD::Seed_t              > *container_seeds      = nullptr;
  std::vector<xAOD::TruthParticle_t     > *container_truth      = nullptr;
  std::vector<xAOD::Electron_t          > *container_electron   = nullptr;

  MSG_DEBUG( "Link all branches..." );

  InitBranch( tree, ("EventInfoContainer_"     + m_outputEventKey).c_str()       , &container_event      );
  InitBranch( tree, ("SeedContainer_"     + m_outputSeedsKey).c_str()       , &container_seeds      );
  InitBranch( tree, ("TruthParticleContainer_" + m_outputTruthKey).c_str()       , &container_truth      );
  InitBranch( tree, ("CaloRingsContainer_"     + m_outputRingerKey).c_str()     , &container_rings       );
  InitBranch( tree, ("CaloClusterContainer_"   + m_outputClusterKey).c_str()     , &container_clus       );
  InitBranch( tree, ("ElectronContainer_"      + m_outputElectronKey).c_str()     , &container_electron  );
  
  if(m_dumpCells){
    InitBranch( tree,  ("CaloCellContainer_"          + m_outputCellsKey).c_str()   , &container_cells      );
    InitBranch( tree,  ("CaloDetDescriptorContainer_" + m_outputCellsKey).c_str()   , &container_descriptor );
  }


  { // serialize EventInfo

    SG::ReadHandle<xAOD::EventInfoContainer> event(m_inputEventKey, ctx);

    if( !event.isValid() ){
      MSG_FATAL( "It's not possible to read the xAOD::EventInfoContainer from this Context" );
    }

    xAOD::EventInfo_t event_t;
    xAOD::EventInfoConverter cnv;
    cnv.convert( (**event.ptr()).front(), event_t);
    container_event->push_back(event_t);
  }
  

  { // Serialize Truth Particle

    SG::ReadHandle<xAOD::TruthParticleContainer> container( m_inputTruthKey, ctx );
  
    if( !container.isValid() )
    {
      MSG_FATAL("It's not possible to read the xAOD::TruthParticleContainer from this Context using this key " << m_inputTruthKey );
    }

    for (const auto par : **container.ptr() ){
      xAOD::TruthParticle_t par_t;
      xAOD::TruthParticleConverter cnv;
      cnv.convert( par, par_t );
      container_truth->push_back(par_t);
    }
  
  }


  { // Serialize Seed
    MSG_DEBUG("Serialize Seed..");
    SG::ReadHandle<xAOD::SeedContainer> container( m_inputSeedsKey, ctx );

    if( !container.isValid() )
    {
      MSG_FATAL("It's not possible to read the xAOD::SeedContainer from this Context using this key " << m_inputSeedsKey );
    }

    for (const auto seed : **container.ptr() ){
      xAOD::Seed_t seed_t;
      xAOD::SeedConverter cnv;
      cnv.convert( seed, seed_t );
      container_seeds->push_back(seed_t);
    }
  }



  {// serialize cluster
    MSG_DEBUG("Serialize Cluster...");
    SG::ReadHandle<xAOD::CaloClusterContainer> container( m_inputClusterKey, ctx );
  
    if( !container.isValid() )
    {
      MSG_WARNING("It's not possible to read the xAOD::CaloClusterContainer from this Context using this key " << m_inputClusterKey );
      return StatusCode::SUCCESS;
    }


    for (const auto &clus : **container.ptr() )
    {

      if(m_dumpCells){

        for(const auto&cell : clus->cells()){
         
          { // serialize cell
            xAOD::CaloCell_t cell_t;
            xAOD::CaloCellConverter cnv;
            cnv.convert(cell, cell_t);
            container_cells->push_back(cell_t);
          }
          {
            const xAOD::CaloDetDescriptor *det = cell->descriptor();
            xAOD::CaloDetDescriptor_t det_t;
            xAOD::CaloDetDescriptorConverter cnv;
            cnv.convert(det, det_t);
            container_descriptor->push_back(det_t);
          }
          
        }
      }

      xAOD::CaloCluster_t clus_t;
      xAOD::CaloClusterConverter cnv;
      cnv.convert( clus , clus_t );
      container_clus->push_back(clus_t);
    }

  }



  {// serialize rings
    MSG_DEBUG("Serialize Rings...");
    SG::ReadHandle<xAOD::CaloRingsContainer> container( m_inputRingerKey, ctx );

    if( !container.isValid() )
    {
      MSG_WARNING("It's not possible to read the xAOD::CaloRingsContainer from this Context using this key " << m_inputRingerKey );
      return StatusCode::SUCCESS;

    }

    for (const auto rings : **container.ptr() ){
      xAOD::CaloRings_t rings_t;
      xAOD::CaloRingsConverter cnv;
      cnv.convert( rings , rings_t);
      container_rings->push_back(rings_t);  
    }

  }

  {
    MSG_DEBUG("Serialize Electrons...");
    SG::ReadHandle<xAOD::ElectronContainer> container( m_inputElectronKey, ctx );
    if (!container.isValid() )
    {
      MSG_WARNING("It's not possible to read the xAOD::ElectronContainer using this key " << m_inputElectronKey);
      return StatusCode::SUCCESS;
    }

    for (const auto electron : **container.ptr() ){
      xAOD::Electron_t electron_t;
      xAOD::ElectronConverter cnv;
      cnv.convert(electron, electron_t);
      container_electron->push_back(electron_t);
    }
  }

  
  tree->Fill();

  if(m_dumpCells){
    delete container_descriptor;
    delete container_cells     ;
  }

  delete container_clus      ;
  delete container_rings     ;
  delete container_event     ;
  delete container_truth     ;
  delete container_seeds     ;
  delete container_electron  ;

  return StatusCode::SUCCESS;
 
}


