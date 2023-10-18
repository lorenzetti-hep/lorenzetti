#include "CaloCell/CaloCellContainer.h"
#include "CaloCluster/CaloClusterContainer.h"
#include "CaloRings/CaloRingsContainer.h"
#include "EventInfo/EventInfoContainer.h"
#include "TruthParticle/TruthParticleContainer.h"

#include "CaloCell/CaloCellConverter.h"
#include "CaloCell/CaloDetDescriptorConverter.h"
#include "CaloCluster/CaloClusterConverter.h"
#include "CaloRings/CaloRingsConverter.h"
#include "EventInfo/EventInfoConverter.h"
#include "EventInfo/EventSeedConverter.h"
#include "TruthParticle/TruthParticleConverter.h"
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

  declareProperty( "OutputEventKey"          , m_outputEventKey="EventInfo"           );
  declareProperty( "OutputSeedsKey"          , m_outputSeedsKey="Seeds"               );
  declareProperty( "OutputTruthKey"          , m_outputTruthKey="Particles"           );
  declareProperty( "OutputCellsKey"          , m_outputCellsKey="Cells"               );
  declareProperty( "OutputClusterKey"        , m_outputClusterKey="Clusters"          );
  declareProperty( "OutputRingerKey"         , m_outputRingerKey="Rings"              );

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
  std::vector<xAOD::EventSeed_t           > container_seeds;
  std::vector<xAOD::TruthParticle_t       > container_truth;

  store->cd();
  TTree *tree = new TTree(m_ntupleName.c_str(), "");

  tree->Branch( ("EventInfoContainer_"     + m_outputEventKey).c_str()       , &container_event      );
  tree->Branch( ("EventSeedContainer_"     + m_outputSeedsKey).c_str()       , &container_seeds      );
  tree->Branch( ("TruthParticleContainer_" + m_outputTruthKey).c_str()       , &container_truth      );
  tree->Branch( ("CaloRingsContainer_"     + m_outputRingerKey).c_str()      , &container_rings      );
  tree->Branch( ("CaloClusterContainer_"   + m_outputClusterKey).c_str()     , &container_clus       );
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
  std::vector<xAOD::EventSeed_t         > *container_seeds      = nullptr;
  std::vector<xAOD::TruthParticle_t     > *container_truth      = nullptr;

  MSG_DEBUG( "Link all branches..." );

  InitBranch( tree, ("EventInfoContainer_"     + m_outputEventKey).c_str()       , &container_event      );
  InitBranch( tree, ("EventSeedContainer_"     + m_outputSeedsKey).c_str()       , &container_seeds      );
  InitBranch( tree, ("TruthParticleContainer_" + m_outputTruthKey).c_str()       , &container_truth      );
  InitBranch( tree, ("CaloRingsContainer_"     + m_outputRingerKey).c_str()     , &container_rings      );
  InitBranch( tree, ("CaloClusterContainer_"   + m_outputClusterKey).c_str()     , &container_clus       );
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
  
  xAOD::cell_links_t       cell_links;
  xAOD::descriptor_links_t descriptor_links;
  xAOD::cluster_links_t    cluster_links;

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
    SG::ReadHandle<xAOD::EventSeedContainer> container( m_inputSeedsKey, ctx );

    if( !container.isValid() )
    {
      MSG_FATAL("It's not possible to read the xAOD::EventSeedContainer from this Context using this key " << m_inputSeedsKey );
    }

    for (const auto seed : **container.ptr() ){
      xAOD::EventSeed_t seed_t;
      xAOD::EventSeedConverter cnv;
      cnv.convert( seed, seed_t );
      container_seeds->push_back(seed_t);
    }
  }



  {// serialize cluster
    
    SG::ReadHandle<xAOD::CaloClusterContainer> container( m_inputClusterKey, ctx );
  
    if( !container.isValid() )
    {
      MSG_WARNING("It's not possible to read the xAOD::CaloClusterContainer from this Context using this key " << m_inputClusterKey );
      return StatusCode::SUCCESS;
    }


    int cluster_link = 0;
    for (const auto &clus : **container.ptr() )
    {

      if(m_dumpCells){

        int cell_link=0;
        for(const auto&cell : clus->cells()){
          if(!cell_links.count(cell))
          {
            const xAOD::CaloDetDescriptor *det = cell->descriptor();

            cell_links[cell]=cell_link;
            descriptor_links[det]=cell_link;


            { // serialize cell
              xAOD::CaloCell_t cell_t;
              xAOD::CaloCellConverter cnv;
              cnv.convert(cell, cell_t, cell_link);
              container_cells->push_back(cell_t);
            }

            {
              xAOD::CaloDetDescriptor_t det_t;
              xAOD::CaloDetDescriptorConverter cnv;
              cnv.convert(det, det_t, cell_link);
              container_descriptor->push_back(det_t);
            }

            cell_link++;
          }
        }
      }

      cluster_links[clus] = cluster_link; // decorate the cluster since is used by rings
      xAOD::CaloCluster_t clus_t;
      xAOD::CaloClusterConverter cnv;
      cnv.convert( clus , clus_t , cell_links);
      container_clus->push_back(clus_t);
      cluster_link++;
    }

  }



  {// serialize rings
    SG::ReadHandle<xAOD::CaloRingsContainer> container( m_inputRingerKey, ctx );

    if( !container.isValid() )
    {
      MSG_WARNING("It's not possible to read the xAOD::CaloRingsContainer from this Context using this key " << m_inputRingerKey );
      return StatusCode::SUCCESS;

    }

    for (const auto rings : **container.ptr() ){
      xAOD::CaloRings_t rings_t;
      xAOD::CaloRingsConverter cnv;
      cnv.convert( rings , rings_t , cluster_links);
      container_rings->push_back(rings_t);  
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

  return StatusCode::SUCCESS;
 
}


