#include "CaloCell/CaloCellContainer.h"
#include "EventInfo/EventInfoContainer.h"
#include "TruthParticle/TruthParticleContainer.h"
#include "TruthParticle/ParticleSeedContainer.h"
#include "CaloCell/CaloCellConverter.h"
#include "CaloCell/CaloDetDescriptorConverter.h"
#include "EventInfo/EventInfoConverter.h"
#include "TruthParticle/TruthParticleConverter.h"
#include "TruthParticle/ParticleSeedConverter.h"
#include "RootStreamESDReader.h"
#include "GaugiKernel/EDM.h"


using namespace SG;
using namespace Gaugi;



RootStreamESDReader::RootStreamESDReader( std::string name ) : 
  IMsgService(name),
  Algorithm()
{
  declareProperty( "InputFile"          , m_inputFile=""                    );
  declareProperty( "EventKey"           , m_eventKey="EventInfo"            );
  declareProperty( "TruthKey"           , m_truthKey="Particles"            );
  declareProperty( "SeedsKey"           , m_seedsKey="Seeds"                );
  declareProperty( "CellsKey"           , m_cellsKey="Cells"                );
  declareProperty( "XTCellsKey"         , m_xtcellsKey="XTCells"            );
  declareProperty( "OutputLevel"        , m_outputLevel=1                   );
  declareProperty( "NtupleName"         , m_ntupleName="CollectionTree"     );
  declareProperty( "DoCrosstalk"        , m_doCrosstalk=false               );
}

//!=====================================================================

RootStreamESDReader::~RootStreamESDReader()
{}

//!=====================================================================

StatusCode RootStreamESDReader::initialize()
{
  CHECK_INIT();
  setMsgLevel(m_outputLevel);
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamESDReader::finalize()
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamESDReader::bookHistograms( EventContext &ctx ) const
{
  auto store = ctx.getStoreGateSvc();
  TFile *file = new TFile(m_inputFile.c_str(), "read");
  store->decorate( "events", file );
  return StatusCode::SUCCESS; 
}

//!=====================================================================

StatusCode RootStreamESDReader::pre_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamESDReader::execute( EventContext &/*ctx*/, const G4Step * /*step*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamESDReader::execute( EventContext &ctx, int evt ) const
{
  return deserialize( evt, ctx );
}

//!=====================================================================

StatusCode RootStreamESDReader::post_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamESDReader::fillHistograms( EventContext &ctx ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamESDReader::deserialize( int evt, EventContext &ctx ) const
{
  std::vector<xAOD::CaloDetDescriptor_t > *collection_descriptor    = nullptr;
  std::vector<xAOD::CaloCell_t          > *collection_cells         = nullptr;
  std::vector<xAOD::CaloDetDescriptor_t > *collection_xtdescriptor  = nullptr;
  std::vector<xAOD::CaloCell_t          > *collection_xtcells       = nullptr;
  std::vector<xAOD::EventInfo_t         > *collection_event         = nullptr;
  std::vector<xAOD::TruthParticle_t     > *collection_truth         = nullptr;
  std::vector<xAOD::ParticleSeed_t      > *collection_seeds         = nullptr;

  MSG_DEBUG( "Link all branches..." );

  auto store = ctx.getStoreGateSvc();
  TFile *file = (TFile*)store->decorator("events");
  TTree *tree = (TTree*)file->Get(m_ntupleName.c_str());

  InitBranch( tree, ("EventInfoContainer_"         + m_eventKey).c_str() , &collection_event     );
  InitBranch( tree, ("TruthParticleContainer_"     + m_truthKey).c_str() , &collection_truth     );
  InitBranch( tree, ("ParticleSeedContainer_"      + m_seedsKey).c_str() , &collection_seeds     );
  InitBranch( tree, ("CaloCellContainer_"          + m_cellsKey).c_str() , &collection_cells     );
  InitBranch( tree, ("CaloDetDescriptorContainer_" + m_cellsKey).c_str() , &collection_descriptor);
  if (m_doCrosstalk){
    InitBranch( tree, ("CaloCellContainer_"          + m_xtcellsKey).c_str() , &collection_xtcells     );
    InitBranch( tree, ("CaloDetDescriptorContainer_" + m_xtcellsKey).c_str() , &collection_xtdescriptor);
  }

  tree->GetEntry( evt );


  { // deserialize Truth Particles
    SG::WriteHandle<xAOD::TruthParticleContainer> container(m_truthKey, ctx);
    container.record( std::unique_ptr<xAOD::TruthParticleContainer>(new xAOD::TruthParticleContainer()));

    xAOD::TruthParticleConverter cnv;
    for( auto& par_t : *collection_truth)
    {
      xAOD::TruthParticle  *par=nullptr;
      cnv.convert(par_t, par);
      MSG_INFO( "Particle in eta = " << par->eta() << ", phi = " << par->phi());
      container->push_back(par);
    }
  }


  { // deserialize Particle Seeds
    SG::WriteHandle<xAOD::ParticleSeedContainer> container(m_seedsKey, ctx);
    container.record( std::unique_ptr<xAOD::ParticleSeedContainer>(new xAOD::ParticleSeedContainer()));

    xAOD::ParticleSeedConverter cnv;
    for( auto& par_t : *collection_seeds)
    {
      xAOD::ParticleSeed  *par=nullptr;
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
    // deserialize Cells
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




    // deserialize XT Cells
    if (m_doCrosstalk){
      MSG_INFO("Reading crosstalk cells with key "<< m_xtcellsKey <<" (DoCrosstalk="<<m_doCrosstalk <<").");
      std::map<int, xAOD::CaloDetDescriptor*> xtdescriptor_links;

      int xtlink=0;
      for (auto &descriptor_t : *collection_xtdescriptor )
      {
        xAOD::CaloDetDescriptor *descriptor = nullptr;
        xAOD::CaloDetDescriptorConverter cnv;
        cnv.convert(descriptor_t, descriptor); // alloc memory
        xtdescriptor_links[xtlink] = descriptor;
        xtlink++;
      }

      SG::WriteHandle<xAOD::CaloCellContainer> container(m_xtcellsKey, ctx);
      container.record( std::unique_ptr<xAOD::CaloCellContainer>(new xAOD::CaloCellContainer()));

      for( auto &cell_t : *collection_xtcells )
      {
        xAOD::CaloCell *cell = nullptr;
        xAOD::CaloCellConverter cnv;
        cnv.convert(cell_t, cell); // alloc memory
        cell->setDescriptor( xtdescriptor_links[cell_t.descriptor_link] );
        container->push_back(cell);
      }
    } // end-if doCrosstalk
  }


  if (m_doCrosstalk){
    delete collection_xtcells       ;
    delete collection_xtdescriptor  ;
  }
  delete collection_descriptor;
  delete collection_cells     ;
  delete collection_event     ;
  delete collection_truth     ;

  return StatusCode::SUCCESS;
 
}

//!=====================================================================

template <class T>
void RootStreamESDReader::InitBranch(TTree* fChain, std::string branch_name, T* param) const
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

