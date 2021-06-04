#include "CaloHit/CaloHitContainer.h"
#include "EventInfo/EventInfoContainer.h"
#include "TruthParticle/TruthParticleContainer.h"
#include "CaloHit/CaloHitConverter.h"
#include "EventInfo/EventInfoConverter.h"
#include "TruthParticle/TruthParticleConverter.h"
#include "TTree.h"
#include "RootStreamHITMaker.h"
#include "GaugiKernel/EDM.h"
#include "G4Kernel/CaloPhiRange.h"



using namespace SG;
using namespace Gaugi;



RootStreamHITMaker::RootStreamHITMaker( std::string name ) : 
  IMsgService(name),
  Algorithm()
{
  declareProperty( "EventKey"           , m_eventKey="EventInfo"            );
  declareProperty( "TruthKey"           , m_truthKey="Particles"            );
  declareProperty( "HitsKey"            , m_hitsKey="hits"                  );
  declareProperty( "OutputLevel"        , m_outputLevel=1                   );
  declareProperty( "NtupleName"         , m_ntupleName="CollectionTree"     );
  //declareProperty( "EtaWindow"          , m_etaWindow=0.6                   );
  //declareProperty( "PhiWindow"          , m_phiWindow=0.6                   );

}




RootStreamHITMaker::~RootStreamHITMaker()
{;}


StatusCode RootStreamHITMaker::initialize()
{
  setMsgLevel(m_outputLevel);
  return StatusCode::SUCCESS;
}


StatusCode RootStreamHITMaker::bookHistograms( SG::EventContext &ctx ) const
{

  auto store = ctx.getStoreGateSvc();

  std::vector<xAOD::CaloHit_t             > container_hits;
  std::vector<xAOD::EventInfo_t           > container_event;
  std::vector<xAOD::TruthParticle_t       > container_truth;

  store->cd();
  TTree *tree = new TTree(m_ntupleName.c_str(), "");
  tree->Branch( "EventInfoContainer"          , &container_event     );
  tree->Branch( "TruthParticleContainer"      , &container_truth     );
  tree->Branch( "CaloHitContainer"           , &container_hits       );
  store->add( tree );
  
  return StatusCode::SUCCESS;
}



StatusCode RootStreamHITMaker::pre_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}


StatusCode RootStreamHITMaker::execute( EventContext &/*ctx*/, const G4Step * /*step*/ ) const
{
  return StatusCode::SUCCESS;
}


StatusCode RootStreamHITMaker::finalize()
{
  return StatusCode::SUCCESS;
}


StatusCode RootStreamHITMaker::post_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}


StatusCode RootStreamHITMaker::fillHistograms( EventContext &ctx ) const
{
  return serialize(ctx);
}


template <class T>
void RootStreamHITMaker::InitBranch(TTree* fChain, std::string branch_name, T* param) const
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




StatusCode RootStreamHITMaker::serialize( EventContext &ctx ) const
{
  

  auto store = ctx.getStoreGateSvc();

  store->cd();
  TTree *tree = store->tree(m_ntupleName);
 
  std::vector<xAOD::CaloHit_t           > *container_hits       = nullptr;
  std::vector<xAOD::EventInfo_t         > *container_event      = nullptr;
  std::vector<xAOD::TruthParticle_t     > *container_truth      = nullptr;

  MSG_DEBUG( "Link all branches..." );

  InitBranch( tree,  "EventInfoContainer"            , &container_event      );
  InitBranch( tree,  "TruthParticleContainer"        , &container_truth      );
  InitBranch( tree,  "CaloHitContainer"              , &container_hits       );


  { // serialize EventInfo
    MSG_DEBUG("Serialize EventInfo...");
    SG::ReadHandle<xAOD::EventInfoContainer> event(m_eventKey, ctx);

    if( !event.isValid() ){
      MSG_FATAL( "It's not possible to read the xAOD::EventInfoContainer from this Context" );
    }

    xAOD::EventInfo_t event_t;
    xAOD::EventInfoConverter cnv;
    cnv.convert( (**event.ptr()).front(), event_t);
    container_event->push_back(event_t);
  }
  


  {
    MSG_DEBUG("Serialize CaloHits...");

    SG::ReadHandle<xAOD::CaloHitContainer> container(m_hitsKey, ctx);
    if( !container.isValid() )
    {
        MSG_FATAL("It's not possible to read the xAOD::CaloHitContainer from this Contaxt using this key " << m_hitsKey );
    }
    float etot=0;
    for (const auto hit : **container.ptr() ){
         
      xAOD::CaloHit_t hit_t;
      xAOD::CaloHitConverter hit_cnv;
      hit_cnv.convert(hit, hit_t);
      container_hits->push_back(hit_t);
      etot+=hit->edep();

    }// check if hit is inside of the window

    MSG_DEBUG("Container hit size is " << container_hits->size() << " and total energy " << etot << " MeV");

  }





  { // Serialize Truth Particle
    MSG_DEBUG("Serialize TruthParticle...");
    SG::ReadHandle<xAOD::TruthParticleContainer> container( m_truthKey, ctx );
  
    if( !container.isValid() )
    {
      MSG_FATAL("It's not possible to read the xAOD::TruthParticleContainer from this Context using this key " << m_truthKey );
    }

    for (const auto par : **container.ptr() ){
      xAOD::TruthParticle_t par_t;
      xAOD::TruthParticleConverter cnv;
      cnv.convert( par, par_t );
      container_truth->push_back(par_t);
    }
  
  }
  
  tree->Fill();

  return StatusCode::SUCCESS;
 
}


