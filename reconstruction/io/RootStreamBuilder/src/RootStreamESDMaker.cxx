#include "CaloCell/CaloCellContainer.h"
#include "EventInfo/EventInfoContainer.h"
#include "TruthParticle/TruthParticleContainer.h"
#include "CaloCell/CaloCellConverter.h"
#include "CaloCell/CaloDetDescriptorConverter.h"
#include "EventInfo/EventInfoConverter.h"
#include "TruthParticle/TruthParticleConverter.h"
#include "EventInfo/EventSeedConverter.h"
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
  declareProperty( "InputCellsKey"      , m_inputCellsKey="Cells"           );
  declareProperty( "InputSeedsKey"      , m_inputSeedsKey="Seeds"           );


  declareProperty( "OutputEventKey"     , m_outputEventKey="EventInfo"      );
  declareProperty( "OutputTruthKey"     , m_outputTruthKey="Particles"      );
  declareProperty( "OutputCellsKey"     , m_outputCellsKey="Cells"          );
  declareProperty( "OutputSeedsKey"     , m_outputSeedsKey="Seeds"          );

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

  std::vector<xAOD::CaloCell_t            > container_cells;
  std::vector<xAOD::CaloDetDescriptor_t   > container_descriptor;
  std::vector<xAOD::EventInfo_t           > container_event;
  std::vector<xAOD::EventSeed_t           > container_seeds;
  std::vector<xAOD::TruthParticle_t       > container_truth;

  store->cd();
  TTree *tree = new TTree(m_ntupleName.c_str(), "");
  tree->Branch( ("EventInfoContainer_"         + m_outputEventKey).c_str() , &container_event     );
  tree->Branch( ("EventSeedContainer_"         + m_outputSeedsKey).c_str() , &container_seeds     );
  tree->Branch( ("TruthParticleContainer_"     + m_outputTruthKey).c_str() , &container_truth     );
  tree->Branch( ("CaloCellContainer_"          + m_outputCellsKey).c_str() , &container_cells     );
  tree->Branch( ("CaloDetDescriptorContainer_" + m_outputCellsKey).c_str() , &container_descriptor);
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
 
  std::vector<xAOD::CaloDetDescriptor_t > *container_descriptor = nullptr;
  std::vector<xAOD::CaloCell_t          > *container_cells      = nullptr;
  std::vector<xAOD::EventInfo_t         > *container_event      = nullptr;
  std::vector<xAOD::EventSeed_t         > *container_seeds      = nullptr;
  std::vector<xAOD::TruthParticle_t     > *container_truth      = nullptr;

  MSG_DEBUG( "Link all branches..." );

  InitBranch( tree, ("EventInfoContainer_"         + m_outputEventKey).c_str() , &container_event     );
  InitBranch( tree, ("EventSeedContainer_"         + m_outputSeedsKey).c_str() , &container_seeds     );
  InitBranch( tree, ("TruthParticleContainer_"     + m_outputTruthKey).c_str() , &container_truth     );
  InitBranch( tree, ("CaloCellContainer_"          + m_outputCellsKey).c_str() , &container_cells     );
  InitBranch( tree, ("CaloDetDescriptorContainer_" + m_outputCellsKey).c_str() , &container_descriptor);

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


  {
    MSG_DEBUG("Serialize CaloCells...");
    xAOD::cell_links_t       cell_links;

    SG::ReadHandle<xAOD::CaloCellContainer> container(m_inputCellsKey, ctx);
    if( !container.isValid() )
    {
        MSG_FATAL("It's not possible to read the xAOD::CaloCellContainer from this Contaxt using this key " << m_inputCellsKey );
    }

    SG::ReadHandle<xAOD::TruthParticleContainer> particles( m_inputTruthKey, ctx );
  
    if( !particles.isValid() )
    {
      MSG_FATAL("It's not possible to read the xAOD::TruthParticleContainer from this Context using this key " << m_inputTruthKey );
    }

    int link = 0; // decorate all cells 

    for (const auto par : **particles.ptr() )
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
  
  tree->Fill();


  delete container_descriptor ;
  delete container_cells      ;
  delete container_seeds      ;
  delete container_event      ;
  delete container_truth      ;

  return StatusCode::SUCCESS;
 
}


