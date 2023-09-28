#include "CaloCell/CaloCellContainer.h"
#include "CaloCluster/CaloClusterContainer.h"
#include "CaloRings/CaloRingsContainer.h"
#include "EventInfo/EventInfoContainer.h"
#include "TruthParticle/TruthParticleContainer.h"
#include "TruthParticle/ParticleSeedContainer.h"

#include "CaloCell/CaloCellConverter.h"
#include "CaloCell/CaloDetDescriptorConverter.h"
#include "CaloCluster/CaloClusterConverter.h"
#include "CaloRings/CaloRingsConverter.h"
#include "EventInfo/EventInfoConverter.h"
#include "TruthParticle/TruthParticleConverter.h"
#include "TruthParticle/ParticleSeedConverter.h"
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
  declareProperty( "InputTruthKey"           , m_inputTruthKey="Particles"            );
  declareProperty( "InputSeedsKey"           , m_inputSeedsKey="Seeds"                );
  declareProperty( "InputCellsKey"           , m_inputCellsKey="Cells"                );
  declareProperty( "InputClusterKey"         , m_inputClusterKey="Clusters"           );
  declareProperty( "InputRingerKey"          , m_inputRingerKey="Rings"               );
  declareProperty( "InputXTCellsKey"         , m_inputXTCellsKey="XTCells"                );
  declareProperty( "InputXTClusterKey"       , m_inputXTClusterKey="XTClusters"           );
  declareProperty( "InputXTRingerKey"        , m_inputXTRingerKey="XTRings"               );

  declareProperty( "OutputEventKey"          , m_outputEventKey="EventInfo"           );
  declareProperty( "OutputTruthKey"          , m_outputTruthKey="Particles"           );
  declareProperty( "OutputSeedsKey"          , m_outputSeedsKey="Seeds"               );
  declareProperty( "OutputCellsKey"          , m_outputCellsKey="Cells"               );
  declareProperty( "OutputClusterKey"        , m_outputClusterKey="Clusters"          );
  declareProperty( "OutputRingerKey"         , m_outputRingerKey="Rings"              );
  declareProperty( "OutputXTCellsKey"        , m_outputXTCellsKey="XTCells"               );
  declareProperty( "OutputXTClusterKey"      , m_outputXTClusterKey="XTClusters"          );
  declareProperty( "OutputXTRingerKey"       , m_outputXTRingerKey="XTRings"              );

  declareProperty( "OutputLevel"             , m_outputLevel=1                        );
  declareProperty( "NtupleName"              , m_ntupleName="physics"                 );
  declareProperty( "DumpCells"               , m_dumpCells=false                      );
  declareProperty( "DoCrosstalk"             , m_doCrosstalk=false                    );
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

  std::vector<xAOD::CaloCell_t            > container_cells, container_xtcells;
  std::vector<xAOD::CaloDetDescriptor_t   > container_descriptor, container_xtdescriptor;
  std::vector<xAOD::CaloCluster_t         > container_clus, container_xtclus;
  std::vector<xAOD::CaloRings_t           > container_rings, container_xtrings;
  std::vector<xAOD::EventInfo_t           > container_event;
  std::vector<xAOD::TruthParticle_t       > container_truth;
  std::vector<xAOD::ParticleSeed_t        > container_seeds;

  store->cd();
  TTree *tree = new TTree(m_ntupleName.c_str(), "");

  tree->Branch( ("EventInfoContainer_"     + m_outputEventKey).c_str()       , &container_event      );
  tree->Branch( ("TruthParticleContainer_" + m_outputTruthKey).c_str()       , &container_truth      );
  tree->Branch( ("ParticleSeedContainer_"  + m_outputSeedsKey).c_str()       , &container_seeds      );
  tree->Branch( ("CaloRingsContainer_"     + m_outputRingerKey).c_str()      , &container_rings      );
  tree->Branch( ("CaloClusterContainer_"   + m_outputClusterKey).c_str()     , &container_clus       );
  if(m_doCrosstalk){
    tree->Branch( ("CaloRingsContainer_"     + m_outputXTRingerKey).c_str()      , &container_xtrings      );
    tree->Branch( ("CaloClusterContainer_"   + m_outputXTClusterKey).c_str()     , &container_xtclus       );
  }
  if(m_dumpCells){
    tree->Branch(  ("CaloCellContainer_"          + m_outputCellsKey).c_str()   , &container_cells      );
    tree->Branch(  ("CaloDetDescriptorContainer_" + m_outputCellsKey).c_str()   , &container_descriptor );
    if(m_doCrosstalk){
      tree->Branch(  ("CaloCellContainer_"          + m_outputXTCellsKey).c_str()   , &container_xtcells      );
      tree->Branch(  ("CaloDetDescriptorContainer_" + m_outputXTCellsKey).c_str()   , &container_xtdescriptor );
    }
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
 
  std::vector<xAOD::CaloDetDescriptor_t > *container_descriptor   = nullptr;
  std::vector<xAOD::CaloDetDescriptor_t > *container_xtdescriptor = nullptr;
  std::vector<xAOD::CaloCell_t          > *container_cells        = nullptr;
  std::vector<xAOD::CaloCell_t          > *container_xtcells      = nullptr;
  std::vector<xAOD::CaloCluster_t       > *container_clus         = nullptr;
  std::vector<xAOD::CaloCluster_t       > *container_xtclus       = nullptr;
  std::vector<xAOD::CaloRings_t         > *container_rings        = nullptr;
  std::vector<xAOD::CaloRings_t         > *container_xtrings      = nullptr;
  std::vector<xAOD::EventInfo_t         > *container_event        = nullptr;
  std::vector<xAOD::TruthParticle_t     > *container_truth        = nullptr;
  std::vector<xAOD::ParticleSeed_t      > *container_seeds        = nullptr;

  MSG_DEBUG( "Link all branches..." );

  InitBranch( tree, ("EventInfoContainer_"     + m_outputEventKey).c_str()       , &container_event      );
  InitBranch( tree, ("TruthParticleContainer_" + m_outputTruthKey).c_str()       , &container_truth      );
  InitBranch( tree, ("ParticleSeedContainer_"  + m_outputSeedsKey).c_str()       , &container_seeds      );
  InitBranch( tree, ("CaloRingsContainer_"     + m_outputRingerKey).c_str()     , &container_rings      );
  InitBranch( tree, ("CaloClusterContainer_"   + m_outputClusterKey).c_str()     , &container_clus       );
  if(m_doCrosstalk){
    InitBranch( tree, ("CaloRingsContainer_"     + m_outputXTRingerKey).c_str()     , &container_xtrings      );
    InitBranch( tree, ("CaloClusterContainer_"   + m_outputXTClusterKey).c_str()    , &container_xtclus       );
  }
  if(m_dumpCells){
    InitBranch( tree,  ("CaloCellContainer_"          + m_outputCellsKey).c_str()   , &container_cells      );
    InitBranch( tree,  ("CaloDetDescriptorContainer_" + m_outputCellsKey).c_str()   , &container_descriptor );
    
    if(m_doCrosstalk){
      InitBranch( tree,  ("CaloCellContainer_"          + m_outputXTCellsKey).c_str()   , &container_xtcells      );
      InitBranch( tree,  ("CaloDetDescriptorContainer_" + m_outputXTCellsKey).c_str()   , &container_xtdescriptor );
    }
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
  xAOD::cell_links_t       xtcell_links;
  xAOD::descriptor_links_t descriptor_links;
  xAOD::descriptor_links_t xtdescriptor_links;
  xAOD::cluster_links_t    cluster_links;
  xAOD::cluster_links_t    xtcluster_links;
  

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

  { // Serialize Particle Seed

    SG::ReadHandle<xAOD::ParticleSeedContainer> container( m_inputSeedsKey, ctx );
  
    if( !container.isValid() )
    {
      MSG_FATAL("It's not possible to read the xAOD::ParticleSeedContainer from this Context using this key " << m_inputSeedsKey );
    }

    for (const auto par : **container.ptr() ){
      xAOD::ParticleSeed_t par_t;
      xAOD::ParticleSeedConverter cnv;
      cnv.convert( par, par_t );
      container_seeds->push_back(par_t);
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





  {// serialize XT cluster
    if(m_doCrosstalk){
      SG::ReadHandle<xAOD::CaloClusterContainer> container( m_inputXTClusterKey, ctx );
    
      if( !container.isValid() )
      {
        MSG_WARNING("It's not possible to read the xAOD::CaloClusterContainer from this Context using this key " << m_inputXTClusterKey );
        return StatusCode::SUCCESS;
      }


      int xtcluster_link = 0;
      for (const auto &clus : **container.ptr() )
      {

        if(m_dumpCells){

          int xtcell_link=0;
          for(const auto&cell : clus->cells()){
            if(!xtcell_links.count(cell))
            {
              const xAOD::CaloDetDescriptor *det = cell->descriptor();

              xtcell_links[cell]=xtcell_link;
              xtdescriptor_links[det]=xtcell_link;


              { // serialize cell
                xAOD::CaloCell_t cell_t;
                xAOD::CaloCellConverter cnv;
                cnv.convert(cell, cell_t, xtcell_link);
                container_xtcells->push_back(cell_t);
              }

              {
                xAOD::CaloDetDescriptor_t det_t;
                xAOD::CaloDetDescriptorConverter cnv;
                cnv.convert(det, det_t, xtcell_link);
                container_xtdescriptor->push_back(det_t);
              }

              xtcell_link++;
            }
          }
        }

        xtcluster_links[clus] = xtcluster_link; // decorate the cluster since is used by rings
        xAOD::CaloCluster_t clus_t;
        xAOD::CaloClusterConverter cnv;
        cnv.convert( clus , clus_t , xtcell_links);
        container_xtclus->push_back(clus_t);
        xtcluster_link++;
      }
    } // end-if doCrosstalk

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




    {// serialize XT rings
    if(m_doCrosstalk){
      SG::ReadHandle<xAOD::CaloRingsContainer> container( m_inputXTRingerKey, ctx );

      if( !container.isValid() )
      {
        MSG_WARNING("It's not possible to read the xAOD::CaloRingsContainer from this Context using this key " << m_inputXTRingerKey );
        return StatusCode::SUCCESS;

      }

      for (const auto rings : **container.ptr() ){
        xAOD::CaloRings_t rings_t;
        xAOD::CaloRingsConverter cnv;
        cnv.convert( rings , rings_t , xtcluster_links);
        container_xtrings->push_back(rings_t);  
      }
    }

  }

  
  tree->Fill();

  if(m_dumpCells){
    delete container_descriptor;
    delete container_cells     ;
    if(m_doCrosstalk){
      delete container_xtdescriptor ;
      delete container_xtcells      ;
    }
  }

  delete container_clus      ;
  delete container_rings     ;
  delete container_event     ;
  delete container_truth     ;
  delete container_seeds     ;
  if(m_doCrosstalk){
    delete container_xtclus   ;
    delete container_xtrings  ;
  }

  return StatusCode::SUCCESS;
 
}


