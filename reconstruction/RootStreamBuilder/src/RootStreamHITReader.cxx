

#include "CaloHit/CaloHitContainer.h"
#include "EventInfo/EventInfoContainer.h"
#include "TruthParticle/TruthParticleContainer.h"

#include "CaloHit/CaloHitConverter.h"
#include "EventInfo/EventInfoConverter.h"
#include "TruthParticle/TruthParticleConverter.h"

#include "RootStreamHITReader.h"
#include "GaugiKernel/EDM.h"


using namespace SG;
using namespace Gaugi;



RootStreamHITReader::RootStreamHITReader( std::string name ) : 
  IMsgService(name),
  ComponentReader(),
  m_entries(0)
{
  declareProperty( "InputFile"          , m_inputFile=""                    );
  declareProperty( "EventKey"           , m_eventKey="EventInfo"            );
  declareProperty( "TruthKey"           , m_truthKey="Particles"            );
  declareProperty( "HitsKey"            , m_hitsKey="Hits"                  );
  declareProperty( "OutputLevel"        , m_outputLevel=1                   );
  declareProperty( "NtupleName"         , m_ntupleName="CollectionTree"     );
}




RootStreamHITReader::~RootStreamHITReader()
{
  delete m_file;
  delete m_tree;
}


StatusCode RootStreamHITReader::initialize()
{
  setMsgLevel(m_outputLevel);
  m_file = new TFile(m_inputFile.c_str(), "read");
  m_tree = (TTree*)m_file->Get(m_ntupleName.c_str());
  m_entries = m_tree->GetEntries();
  MSG_INFO( "Got " << m_entries << " Events!");
  return StatusCode::SUCCESS;
}

StatusCode RootStreamHITReader::finalize()
{
  return StatusCode::SUCCESS;
}


StatusCode RootStreamHITReader::GeneratePrimaryVertex( int evt, EventContext &ctx ) const
{
  return deserialize( evt, ctx );
}



template <class T>
void RootStreamHITReader::InitBranch(TTree* fChain, std::string branch_name, T* param) const
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




StatusCode RootStreamHITReader::deserialize( int evt, EventContext &ctx ) const
{
  std::vector<xAOD::CaloHit_t           > *collection_hits      = nullptr;
  std::vector<xAOD::EventInfo_t         > *collection_event      = nullptr;
  std::vector<xAOD::TruthParticle_t     > *collection_truth      = nullptr;

  MSG_DEBUG( "Link all branches..." );

  InitBranch( m_tree,  "EventInfoContainer"            , &collection_event      );
  InitBranch( m_tree,  "TruthParticleContainer"        , &collection_truth      );
  InitBranch( m_tree,  "CaloHitContainer"              , &collection_hits       );

  m_tree->GetEntry( evt );


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
    SG::WriteHandle<xAOD::CaloHitContainer> container(m_hitsKey, ctx);
    container.record( std::unique_ptr<xAOD::CaloHitContainer>(new xAOD::CaloHitContainer()));
    float etot=0;
    for( auto &hit_t : *collection_hits )
    {
      xAOD::CaloHit *hit = nullptr;
      xAOD::CaloHitConverter cnv;
      cnv.convert(hit_t, hit); // alloc memory
      container->push_back(hit);
      etot+=hit->edep();
    }

    MSG_DEBUG("Container hit size is " << container->size() << " and total energy " << etot << " MeV " );
  }

  return StatusCode::SUCCESS;
 
}

int RootStreamHITReader::GetEntries() const
{
  return m_entries;
}

