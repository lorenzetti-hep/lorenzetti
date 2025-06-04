#include "CaloHit/CaloHitContainer.h"
#include "EventInfo/EventInfoContainer.h"
#include "TruthParticle/TruthParticleContainer.h"
#include "CaloHit/CaloHitConverter.h"
#include "EventInfo/EventInfoConverter.h"
#include "TruthParticle/TruthParticleConverter.h"
#include "EventInfo/SeedConverter.h"
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
  declareProperty( "InputEventKey"           , m_inputEventKey="Events"                );
  declareProperty( "InputTruthKey"           , m_inputTruthKey="Particles"             );
  declareProperty( "InputHitsKey"            , m_inputHitsKey="Hits"                   );
  declareProperty( "InputSeedsKey"           , m_inputSeedsKey="Seeds"                 );
  declareProperty( "OutputEventKey"          , m_outputEventKey="EventInfo"            );
  declareProperty( "OutputTruthKey"          , m_outputTruthKey="Particles"            );
  declareProperty( "OutputHitsKey"           , m_outputHitsKey="Hits"                  );
  declareProperty( "OutputSeedsKey"          , m_outputSeedsKey="Seeds"                 );

  declareProperty( "OutputLevel"             , m_outputLevel=1                         );
  declareProperty( "NtupleName"              , m_ntupleName="CollectionTree"           );
  declareProperty( "OnlyRoI"                 , m_onlyRoI=false                         );
  declareProperty( "EtaWindow"               , m_etaWindow=0.6                         );
  declareProperty( "PhiWindow"               , m_phiWindow=0.6                         );
  
  // new for including cell defects
  declareProperty( "doDefects"               , m_doDefects=false                       );
  declareProperty( "cellHash"                , m_cellHash={}                           );                 
}

//!=====================================================================

RootStreamHITMaker::~RootStreamHITMaker()
{;}

//!=====================================================================

StatusCode RootStreamHITMaker::initialize()
{
  CHECK_INIT();
  //setMsgLevel(m_outputLevel);
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamHITMaker::bookHistograms( SG::EventContext &ctx ) const
{

  auto store = ctx.getStoreGateSvc();

  std::vector<xAOD::CaloHit_t             > container_hits;
  std::vector<xAOD::EventInfo_t           > container_event;
  std::vector<xAOD::TruthParticle_t       > container_truth;
  std::vector<xAOD::Seed_t                > container_seeds;

  store->cd();
  TTree *tree = new TTree(m_ntupleName.c_str(), "");
  tree->Branch( ("EventInfoContainer_"+m_outputEventKey).c_str()     , &container_event     );
  tree->Branch( ("SeedContainer_"+m_outputSeedsKey).c_str()     , &container_seeds     );
  tree->Branch( ("TruthParticleContainer_"+m_outputTruthKey).c_str() , &container_truth     );
  tree->Branch( ("CaloHitContainer_"+m_outputHitsKey).c_str()        , &container_hits      );
  
  store->add( tree );
  
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamHITMaker::pre_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamHITMaker::execute( EventContext &/*ctx*/, const G4Step * /*step*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamHITMaker::execute( EventContext &ctx, int /*evt*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamHITMaker::finalize()
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamHITMaker::post_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamHITMaker::fillHistograms( EventContext &ctx ) const
{
  return serialize(ctx);
}

//!=====================================================================

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

//!=====================================================================

StatusCode RootStreamHITMaker::serialize( EventContext &ctx ) const
{

  MSG_DEBUG("Serialize...");
  auto store = ctx.getStoreGateSvc();

  for( auto key : ctx.keys())
  {
    MSG_DEBUG(key);
  }

  store->cd();
  TTree *tree = store->tree(m_ntupleName);
 
  std::vector<xAOD::CaloHit_t           > *container_hits       = nullptr;
  std::vector<xAOD::EventInfo_t         > *container_event      = nullptr;
  std::vector<xAOD::Seed_t              > *container_seeds      = nullptr;
  std::vector<xAOD::TruthParticle_t     > *container_truth      = nullptr;

  MSG_DEBUG( "Link all branches..." );

  InitBranch( tree, ("EventInfoContainer_"+m_outputEventKey).c_str()     , &container_event     );
  InitBranch( tree, ("SeedContainer_"+m_outputSeedsKey).c_str()          , &container_seeds     );
  InitBranch( tree, ("TruthParticleContainer_"+m_outputTruthKey).c_str() , &container_truth     );
  InitBranch( tree, ("CaloHitContainer_"+m_outputHitsKey).c_str()        , &container_hits      );

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
  

  {
    MSG_DEBUG("Serialize CaloHits...");

    SG::ReadHandle<xAOD::TruthParticleContainer> particles( m_inputTruthKey, ctx );

    SG::ReadHandle<xAOD::CaloHitContainer> container(m_inputHitsKey, ctx);
    if( !container.isValid() )
    {
        MSG_FATAL("It's not possible to read the xAOD::CaloHitContainer from this Contaxt using this key " << m_inputHitsKey );
    }

    float etot=0;
    for (const auto hit : **container.ptr() ){
         
      if(m_onlyRoI){
        bool match=false;
        for (const auto& par : **particles.ptr())
        {
          float deltaEta = std::abs( par->eta() - hit->eta() );
          float deltaPhi = std::abs( CaloPhiRange::diff(par->phi(), hit->phi()) );
          if ( deltaEta < m_etaWindow/2 && deltaPhi < m_phiWindow/2 )
          {
            match=true;
            break;
            }
          //   else if (m_doDefects) 
          //   {
          //   // Check if hit->hash() is in any of the vectors inside m_cellHash
          //   for (const auto& defectList : m_cellHash) {
          //     if (std::find(defectList.begin(), defectList.end(), hit->hash()) != defectList.end()) {
          //     match = true; // if hash is on list of defect cell, then we keep the hit
          //     MSG_INFO("Hit with hash " << hit->hash() << " is in the defect list, keeping it.");
          //     break;
          //     }
          //   }
          //   match=true; // if hash is on list of defect cell, then we keep the hit
          //   MSG_INFO("Hit with hash " << hit->hash() << " is in the defect list, keeping it.");
          // }
        }

        if(!match) continue; // skip this hit
      }

      xAOD::CaloHit_t hit_t;
      xAOD::CaloHitConverter hit_cnv;
      hit_cnv.convert(hit, hit_t);

      if(hit->hash() != hit_t.hash){
        MSG_FATAL("Hit missmatch " << hit->hash() << " != " << hit_t.hash);
      }
      
      container_hits->push_back(hit_t);
      etot+=hit->edep();

    }// check if hit is inside of the window

    MSG_DEBUG("Container hit size is " << container_hits->size() << " and total energy " << etot << " MeV");

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

  delete container_hits   ;
  delete container_event  ;
  delete container_truth  ;
  delete container_seeds  ;

  return StatusCode::SUCCESS;
 
}

//!=====================================================================

