#include "CaloCell/CaloCellContainer.h"
#include "EventInfo/EventInfoContainer.h"
#include "TruthParticle/TruthParticleContainer.h"
#include "CaloCell/CaloCellConverter.h"
#include "CaloCell/CaloDetDescriptorConverter.h"
#include "CaloCell/CaloDetDescriptorContainer.h"
#include "EventInfo/EventInfoConverter.h"
#include "TruthParticle/TruthParticleConverter.h"
#include "EventInfo/SeedConverter.h"
#include "RootStreamESDReader.h"
#include "GaugiKernel/EDM.h"


using namespace SG;
using namespace Gaugi;



RootStreamESDReader::RootStreamESDReader( std::string name ) : 
  IMsgService(name),
  Algorithm()
{
  declareProperty( "InputFile"          , m_inputFile=""                    );
  declareProperty( "OutputEventKey"     , m_eventKey="EventInfo"            );
  declareProperty( "OutputTruthKey"     , m_truthKey="Particles"            );
  declareProperty( "OutputCellsKey"     , m_cellsKey="Cells"                );
  declareProperty( "OutputSeedsKey"     , m_seedsKey="Seeds"                );

  declareProperty( "OutputLevel"        , m_outputLevel=1                   );
  declareProperty( "NtupleName"         , m_ntupleName="CollectionTree"     );
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
  std::vector<xAOD::CaloDetDescriptor_t > *collection_descriptor = nullptr;
  std::vector<xAOD::CaloCell_t          > *collection_cells      = nullptr;
  std::vector<xAOD::EventInfo_t         > *collection_event      = nullptr;
  std::vector<xAOD::Seed_t              > *collection_seeds      = nullptr;
  std::vector<xAOD::TruthParticle_t     > *collection_truth      = nullptr;

  MSG_DEBUG( "Link all branches..." );

  auto store = ctx.getStoreGateSvc();
  TFile *file = (TFile*)store->decorator("events");
  TTree *tree = (TTree*)file->Get(m_ntupleName.c_str());

  InitBranch( tree, ("EventInfoContainer_"         + m_eventKey).c_str() , &collection_event     );
  InitBranch( tree, ("SeedContainer_"              + m_seedsKey).c_str() , &collection_seeds     );
  InitBranch( tree, ("TruthParticleContainer_"     + m_truthKey).c_str() , &collection_truth     );
  InitBranch( tree, ("CaloCellContainer_"          + m_cellsKey).c_str() , &collection_cells     );
  InitBranch( tree, ("CaloDetDescriptorContainer_" + m_cellsKey).c_str() , &collection_descriptor);

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
    }
  }
  

  MSG_DEBUG("Deserialize CaloDetDescriptor... ");
  xAOD::descriptor_links_t descriptor_links;
  {
    SG::WriteHandle<xAOD::CaloDetDescriptorContainer> container( m_cellsKey+"_Aux", ctx );
    container.record( std::unique_ptr<xAOD::CaloDetDescriptorContainer>(new xAOD::CaloDetDescriptorContainer()) );
    std::map<unsigned long int,const xAOD::CaloDetDescriptor_t> descriptor_map;
    MSG_DEBUG(collection_descriptor->size());
    for (auto &descriptor_t : *collection_descriptor )
    {

      // NOTE: avoid cell duplication given RoI superposition
      if ( descriptor_map.count(descriptor_t.hash)){
        continue;
      }
      descriptor_map.insert( std::make_pair( descriptor_t.hash, descriptor_t ) );
      //MSG_DEBUG(descriptor_t.hash);
      xAOD::CaloDetDescriptor *descriptor = nullptr;
      xAOD::CaloDetDescriptorConverter cnv;
      cnv.convert(descriptor_t, descriptor); // alloc memory
      descriptor_links[descriptor->hash()] = descriptor;
      container->push_back(descriptor);
    }

  }
  

  MSG_DEBUG("Deserialize CaloCells... ");
  {

    SG::WriteHandle<xAOD::CaloCellContainer> container(m_cellsKey, ctx);
    container.record( std::unique_ptr<xAOD::CaloCellContainer>(new xAOD::CaloCellContainer()));
    std::map<unsigned long int,const xAOD::CaloDetDescriptor*> descriptor_map;
    for( auto &cell_t : *collection_cells )
    {

      if ( descriptor_links.count(cell_t.descriptor_link)  )
      {
        auto descriptor = descriptor_links[cell_t.descriptor_link];

        // NOTE: avoid cell duplication given RoI superposition
        if ( descriptor_map.count(descriptor->hash())){
          MSG_FATAL("There is a cell duplication. Please check the code. Abort");
          continue;
        }

        descriptor_map.insert( std::make_pair( descriptor->hash(), descriptor ) );
        xAOD::CaloCell *cell = nullptr;
        xAOD::CaloCellConverter cnv;
        cnv.convert(cell_t, cell); // alloc memory
        cell->setDescriptor( descriptor );
        container->push_back(cell);
      }  
      
    }

  }
  

  delete collection_descriptor;
  delete collection_seeds     ;
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

