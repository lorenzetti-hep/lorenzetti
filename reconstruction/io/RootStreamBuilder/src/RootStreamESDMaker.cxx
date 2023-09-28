#include "CaloCell/CaloCellContainer.h"
#include "EventInfo/EventInfoContainer.h"
#include "TruthParticle/TruthParticleContainer.h"
#include "TruthParticle/ParticleSeedContainer.h"
#include "CaloCell/CaloCellConverter.h"
#include "CaloCell/CaloDetDescriptorConverter.h"
#include "EventInfo/EventInfoConverter.h"
#include "TruthParticle/TruthParticleConverter.h"
#include "TruthParticle/ParticleSeedConverter.h"
#include "TTree.h"
#include "RootStreamESDMaker.h"
#include "GaugiKernel/EDM.h"
#include "G4Kernel/CaloPhiRange.h"



using namespace SG;
using namespace Gaugi;



RootStreamESDMaker::RootStreamESDMaker( std::string name ) : 
  IMsgService(name),
  Algorithm()
{
  declareProperty( "InputEventKey"      , m_inputEventKey="EventInfo"       );
  declareProperty( "InputTruthKey"      , m_inputTruthKey="Particles"       );
  declareProperty( "InputSeedsKey"      , m_inputSeedsKey="Seeds"           );
  declareProperty( "InputCellsKey"      , m_inputCellsKey="Cells"           );
  declareProperty( "InputXTCellsKey"    , m_inputXTCellsKey="XTCells"       );
  declareProperty( "OutputEventKey"     , m_outputEventKey="EventInfo"      );
  declareProperty( "OutputTruthKey"     , m_outputTruthKey="Particles"      );
  declareProperty( "OutputSeedsKey"     , m_outputSeedsKey="Seeds"          );
  declareProperty( "OutputCellsKey"     , m_outputCellsKey="Cells"          );
  declareProperty( "OutputXTCellsKey"   , m_outputXTCellsKey="XTCells"      );

  declareProperty( "DumpCrossTalkCells" , m_dumpXTCells=false               );

  declareProperty( "OutputLevel"        , m_outputLevel=1                   );
  declareProperty( "NtupleName"         , m_ntupleName="CollectionTree"     );
  declareProperty( "EtaWindow"          , m_etaWindow=0.6                   );
  declareProperty( "PhiWindow"          , m_phiWindow=0.6                   );

}

//!=====================================================================

RootStreamESDMaker::~RootStreamESDMaker()
{;}

//!=====================================================================

StatusCode RootStreamESDMaker::initialize()
{
  CHECK_INIT();
  setMsgLevel(m_outputLevel);
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamESDMaker::bookHistograms( SG::EventContext &ctx ) const
{

  auto store = ctx.getStoreGateSvc();

  std::vector<xAOD::CaloCell_t            > container_cells, container_xtcells;
  std::vector<xAOD::CaloDetDescriptor_t   > container_descriptor, container_xtdescriptor;
  std::vector<xAOD::EventInfo_t           > container_event;
  std::vector<xAOD::TruthParticle_t       > container_truth;
  std::vector<xAOD::ParticleSeed_t        > container_seeds;

  store->cd();
  TTree *tree = new TTree(m_ntupleName.c_str(), "");
  tree->Branch( ("EventInfoContainer_"         + m_outputEventKey).c_str() , &container_event     );
  tree->Branch( ("TruthParticleContainer_"     + m_outputTruthKey).c_str() , &container_truth     );
  tree->Branch( ("ParticleSeedContainer_"      + m_outputSeedsKey).c_str() , &container_seeds     );
  tree->Branch( ("CaloCellContainer_"          + m_outputCellsKey).c_str() , &container_cells     );
  tree->Branch( ("CaloDetDescriptorContainer_" + m_outputCellsKey).c_str() , &container_descriptor);
  if (m_dumpXTCells){
    tree->Branch( ("CaloCellContainer_"          + m_outputXTCellsKey).c_str() , &container_xtcells     );
    tree->Branch( ("CaloDetDescriptorContainer_" + m_outputXTCellsKey).c_str() , &container_xtdescriptor);
    
  }
  store->add( tree );
  
  return StatusCode::SUCCESS;
}


//!=====================================================================

StatusCode RootStreamESDMaker::pre_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamESDMaker::execute( EventContext &/*ctx*/, const G4Step * /*step*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamESDMaker::execute( EventContext &/*ctx*/, int /*evt*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamESDMaker::finalize()
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamESDMaker::post_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamESDMaker::fillHistograms( EventContext &ctx ) const
{
  return serialize(ctx);
}

//!=====================================================================

template <class T>
void RootStreamESDMaker::InitBranch(TTree* fChain, std::string branch_name, T* param) const
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




StatusCode RootStreamESDMaker::serialize( EventContext &ctx ) const
{
  

  auto store = ctx.getStoreGateSvc();

  store->cd();
  TTree *tree = store->tree(m_ntupleName);
 
  std::vector<xAOD::CaloDetDescriptor_t > *container_descriptor   = nullptr;
  std::vector<xAOD::CaloCell_t          > *container_cells        = nullptr;
  std::vector<xAOD::EventInfo_t         > *container_event        = nullptr;
  std::vector<xAOD::TruthParticle_t     > *container_truth        = nullptr;
  std::vector<xAOD::ParticleSeed_t      > *container_seeds        = nullptr;
  std::vector<xAOD::CaloDetDescriptor_t > *container_xtdescriptor = nullptr;
  std::vector<xAOD::CaloCell_t          > *container_xtcells      = nullptr;

  MSG_DEBUG( "Link all branches..." );

  InitBranch( tree, ("EventInfoContainer_"         + m_outputEventKey).c_str() , &container_event     );
  InitBranch( tree, ("TruthParticleContainer_"     + m_outputTruthKey).c_str() , &container_truth     );
  InitBranch( tree, ("ParticleSeedContainer_"      + m_outputSeedsKey).c_str() , &container_seeds     );
  InitBranch( tree, ("CaloCellContainer_"          + m_outputCellsKey).c_str() , &container_cells     );
  InitBranch( tree, ("CaloDetDescriptorContainer_" + m_outputCellsKey).c_str() , &container_descriptor);
  if (m_dumpXTCells){
    InitBranch( tree, ("CaloCellContainer_"          + m_outputXTCellsKey).c_str() , &container_xtcells     );
    InitBranch( tree, ("CaloDetDescriptorContainer_" + m_outputXTCellsKey).c_str() , &container_xtdescriptor);
  }

  { // serialize EventInfo
    MSG_DEBUG("Serialize EventInfo...");
    SG::ReadHandle<xAOD::EventInfoContainer> event(m_inputEventKey, ctx);

    if( !event.isValid() ){
      MSG_FATAL( "It's not possible to read the xAOD::EventInfoContainer from this Context" );
    }

    xAOD::EventInfo_t event_t;
    xAOD::EventInfoConverter cnv;
    cnv.convert( (**event.ptr()).front(), event_t);
    container_event->push_back(event_t);
  }
  


  {
    MSG_DEBUG("Serialize CaloCells...");
    xAOD::cell_links_t       cell_links, xtcell_links;

    SG::ReadHandle<xAOD::CaloCellContainer> container(m_inputCellsKey, ctx);
    if( !container.isValid() )
    {
        MSG_FATAL("It's not possible to read the xAOD::CaloCellContainer from this Contaxt using this key " << m_inputCellsKey );
    }

    // ---- CrossTalk Cells Container ----
    std::string warningSupressXTkey;
    if (m_dumpXTCells) warningSupressXTkey=m_inputXTCellsKey;
    else warningSupressXTkey=m_inputCellsKey;

    SG::ReadHandle<xAOD::CaloCellContainer> xtcontainer(warningSupressXTkey, ctx);

    if( !xtcontainer.isValid() && m_dumpXTCells)  {MSG_FATAL("It's not possible to read the xAOD::CaloCellContainer from this Context using the key " << m_inputXTCellsKey );}
    if( !xtcontainer.isValid() && !m_dumpXTCells) {MSG_WARNING("There will be no CrossTalk Cells in the output ESD file (DumpCrossTalkCells="<<m_dumpXTCells << ").");}
    // ------------------------------------

    // SG::ReadHandle<xAOD::TruthParticleContainer> particles( m_inputTruthKey, ctx );
    SG::ReadHandle<xAOD::ParticleSeedContainer> seeds( m_inputSeedsKey, ctx);
  
    if( !seeds.isValid() )
    {
      MSG_FATAL("It's not possible to read the xAOD::ParticleSeedsContainer from this Context using this key " << m_inputSeedsKey );
    }

    int linkXT = 0, link = 0; // decorate all cells 

    // for (const auto par : **particles.ptr() )
    for (const auto par : **seeds.ptr() )
    {
        //MSG_DEBUG("New particle...");
        for (const auto cell : **container.ptr() ){
            const xAOD::CaloDetDescriptor *descriptor = cell->descriptor();
        
            float deltaEta = std::abs( par->eta() - descriptor->eta());
            float deltaPhi = std::abs( CaloPhiRange::diff(par->phi(), descriptor->phi()) );

            if ( deltaEta < m_etaWindow/2 && deltaPhi < m_phiWindow/2 )
            {
                xAOD::CaloCell_t cell_t;
                xAOD::CaloCellConverter cell_cnv;
                xAOD::CaloDetDescriptor_t descriptor_t;
                xAOD::CaloDetDescriptorConverter descriptor_cnv;


                if (cell_links.count(cell)){
                    cell_cnv.convert(cell, cell_t, cell_links[cell]);
                    descriptor_cnv.convert( descriptor, descriptor_t, cell_links[cell]);
                }else{
                    cell_links[cell] = link;
                    cell_cnv.convert(cell, cell_t, link);
                    descriptor_cnv.convert( descriptor, descriptor_t, link);
                    link++;
                }

                container_cells->push_back(cell_t);
                container_descriptor->push_back(descriptor_t);

            }// check if cell is inside of the window
        
        }// loop over all cells

        // if crosstalk flag is true:
        if (m_dumpXTCells){
          for (const auto cell : **xtcontainer.ptr() ){
            const xAOD::CaloDetDescriptor *descriptor = cell->descriptor();
        
            float deltaEta = std::abs( par->eta() - descriptor->eta());
            float deltaPhi = std::abs( CaloPhiRange::diff(par->phi(), descriptor->phi()) );

            if ( deltaEta < m_etaWindow/2 && deltaPhi < m_phiWindow/2 )
            {
              xAOD::CaloCell_t cell_t;
              xAOD::CaloCellConverter cell_cnv;
              xAOD::CaloDetDescriptor_t descriptor_t;
              xAOD::CaloDetDescriptorConverter descriptor_cnv;


              if (xtcell_links.count(cell)){
                  cell_cnv.convert(cell, cell_t, xtcell_links[cell]);
                  descriptor_cnv.convert( descriptor, descriptor_t, xtcell_links[cell]);
              }else{
                  xtcell_links[cell] = linkXT;
                  cell_cnv.convert(cell, cell_t, linkXT);
                  descriptor_cnv.convert( descriptor, descriptor_t, linkXT);
                  linkXT++;
              }

              container_xtcells->push_back(cell_t);
              container_xtdescriptor->push_back(descriptor_t);

            }// check if XTcell is inside of the window
        
          }// loop over all XTcells

        }

        
    }// loop over all seeds

  }



  { // Serialize Truth Particle
    MSG_DEBUG("Serialize TruthParticle...");
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


  { // Serialize Particle Seeds
    MSG_DEBUG("Serialize ParticleSeeds...");
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
  
  tree->Fill();

  if (m_dumpXTCells){
    delete container_xtcells      ;
    delete container_xtdescriptor ;
  }

  delete container_descriptor ;
  delete container_cells      ;
  delete container_event      ;
  delete container_truth      ;
  delete container_seeds      ;

  return StatusCode::SUCCESS;
 
}
