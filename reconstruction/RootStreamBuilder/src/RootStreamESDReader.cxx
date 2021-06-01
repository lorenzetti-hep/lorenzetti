#include "CaloCell/CaloCellContainer.h"
#include "EventInfo/EventInfoContainer.h"
#include "TruthParticle/TruthParticleContainer.h"
#include "CaloCell/CaloCellConverter.h"
#include "CaloCell/CaloDetDescriptorConverter.h"
#include "EventInfo/EventInfoConverter.h"
#include "TruthParticle/TruthParticleConverter.h"
#include "RootStreamESDReader.h"
#include "GaugiKernel/EDM.h"


using namespace SG;
using namespace Gaugi;



RootStreamESDReader::RootStreamESDReader( std::string name ) : 
  IMsgService(name),
  ComponentReader(),
  m_entries(0)
{
  declareProperty( "InputFile"          , m_inputFile=""                    );
  declareProperty( "EventKey"           , m_eventKey="EventInfo"            );
  declareProperty( "TruthKey"           , m_truthKey="Particles"            );
  declareProperty( "CellsKey"           , m_cellsKey="Cells"                );
  declareProperty( "OutputLevel"        , m_outputLevel=1                   );
  declareProperty( "NtupleName"         , m_ntupleName="CollectionTree"     );
}




RootStreamESDReader::~RootStreamESDReader()
{
  delete m_file;
  delete m_tree;
}


StatusCode RootStreamESDReader::initialize()
{
  setMsgLevel(m_outputLevel);
  m_file = new TFile(m_inputFile.c_str(), "read");
  m_tree = (TTree*)m_file->Get(m_ntupleName.c_str());
  m_entries = m_tree->GetEntries();
  MSG_INFO( "Got " << m_entries << " Events!");
  return StatusCode::SUCCESS;
}

StatusCode RootStreamESDReader::finalize()
{
  return StatusCode::SUCCESS;
}


StatusCode RootStreamESDReader::GeneratePrimaryVertex( int evt, EventContext &ctx ) const
{
  return deserialize( evt, ctx );
}



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




StatusCode RootStreamESDReader::deserialize( int evt, EventContext &ctx ) const
{
  std::vector<xAOD::CaloDetDescriptor_t > *collection_descriptor = nullptr;
  std::vector<xAOD::CaloCell_t          > *collection_cells      = nullptr;
  std::vector<xAOD::EventInfo_t         > *collection_event      = nullptr;
  std::vector<xAOD::TruthParticle_t     > *collection_truth      = nullptr;

  MSG_DEBUG( "Link all branches..." );

  InitBranch( m_tree,  "EventInfoContainer"            , &collection_event      );
  InitBranch( m_tree,  "TruthParticleContainer"        , &collection_truth      );
  InitBranch( m_tree,  "CaloCellContainer"             , &collection_cells      );
  InitBranch( m_tree,  "CaloDetDescriptorContainer"    , &collection_descriptor );

  m_tree->GetEntry( evt );


  { // deserialize EventInfo

    SG::WriteHandle<xAOD::EventInfoContainer> container(m_eventKey, ctx);
    container.record( std::unique_ptr<xAOD::EventInfoContainer>(new xAOD::EventInfoContainer()));

    xAOD::EventInfo  *event=nullptr;
    xAOD::EventInfoConverter cnv;
    cnv.convert(  collection_event->at(0), event);
    container->push_back(event);
  }
  

    { // deserialize EventInfo

    SG::WriteHandle<xAOD::TruthParticleContainer> container(m_truthKey, ctx);
    container.record( std::unique_ptr<xAOD::TruthParticleContainer>(new xAOD::TruthParticleContainer()));

    xAOD::TruthParticleConverter cnv;
    for( auto& par_t : *collection_truth)
    {
      xAOD::TruthParticle  *par=nullptr;
      cnv.convert(par_t, par);
      container->push_back(par);
    }
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

  return StatusCode::SUCCESS;
 
}

int RootStreamESDReader::GetEntries() const
{
  return m_entries;
}

