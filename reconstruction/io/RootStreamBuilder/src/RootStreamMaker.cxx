

#include "CaloHit/CaloHitConverter.h"
#include "CaloCell/CaloCellConverter.h"
#include "EventInfo/EventInfoConverter.h"
#include "EventInfo/EventSeedConverter.h"
#include "TruthParticle/TruthParticleConverter.h"
#include "RootStreamMaker.h"
#include "TTree.h"

#include <boost/algorithm/string.hpp>


using namespace SG;
using namespace Gaugi;
using namespace xAOD;


RootStreamMaker::RootStreamMaker( std::string name ) : 
  IMsgService(name),
  Algorithm()
{
  declareProperty( "Containers"              , m_containers={}                         );
  declareProperty( "OutputLevel"             , m_outputLevel=1                         );
  declareProperty( "NtupleName"              , m_ntupleName="CollectionTree"           );
  declareProperty( "OnlyRoI"                 , m_onlyRoI=false                         );
  declareProperty( "EtaWindow"               , m_etaWindow=0.6                         );
  declareProperty( "PhiWindow"               , m_phiWindow=0.6                         );
}

//!=====================================================================

RootStreamMaker::~RootStreamMaker()
{;}

//!=====================================================================

StatusCode RootStreamMaker::initialize()
{
  CHECK_INIT();
  setMsgLevel(m_outputLevel);
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamMaker::bookHistograms( SG::EventContext &ctx ) const
{
  auto store = ctx.getStoreGateSvc();
  store->cd();
  TTree *tree = new TTree(m_ntupleName.c_str(), "");
  store->add( tree );
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamMaker::pre_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamMaker::execute( EventContext &/*ctx*/, const G4Step * /*step*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamMaker::execute( EventContext &/*ctx*/, int /*evt*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamMaker::finalize()
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamMaker::post_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamMaker::fillHistograms( EventContext &ctx ) const
{
  return serialize(ctx);
}

//!=====================================================================

StatusCode RootStreamMaker::serialize( EventContext &ctx ) const
{
  MSG_INFO("Serialize all containers...");

  auto store = ctx.getStoreGateSvc();
  for( auto key : ctx.keys())
  {
    MSG_INFO(key);
  }

  std::string seedKey = "";

  store->cd();
  TTree *tree = store->tree(m_ntupleName);
 
  for (auto &container_key : m_containers){
    
    MSG_INFO("Preparing " << container_key << "...");

    std::vector<std::string> result;
    boost::split(result, container_key, boost::is_any_of("#") );
    auto container = result.at(0);
    auto key       = result.at(1);
    
    if (container == "xAOD::EventInfoContainer"){

      MSG_INFO("Converting and serializing EventInfo objects into root file...");
      EventInfoConverter cnv;
      if( !cnv.serialize( key, ctx, tree ) ){
        MSG_FATAL("Its not possible to serialize xAOD::EventInfoContainer.");
      };

    }else if (container == "xAOD::EventSeedContainer"){

      MSG_INFO("Converting and serializing EventSeed objects into root file...");
      EventSeedConverter cnv;
      if( !cnv.serialize( key, ctx, tree ) ){
        MSG_FATAL("Its not possible to serialize xAOD::EventSeedContainer.");
      };
      seedKey = key;

    }else if (container == "xAOD::TruthParticleContainer"){

      MSG_INFO("Converting and serializing TruthParticle objects into root file...");
      TruthParticleConverter cnv;
      if( !cnv.serialize( key, ctx, tree ) ){
        MSG_FATAL("Its not possible to serialize xAOD::TruthParticleContainer.");
      };

    }else if (container == "xAOD::CaloCellContainer"){

      MSG_INFO("Converting and serializing CaloCell objects into root file...");
      CaloCellConverter cnv(seedKey , m_etaWindow, m_phiWindow );
      if( !cnv.serialize( key, ctx, tree ) ){
        MSG_FATAL("Its not possible to serialize xAOD::CaloCellContainer.");
      };

    }else if (container == "xAOD::CaloHitContainer"){

      MSG_INFO("Converting and serializing CaloHit objects into root file...");
      CaloHitConverter cnv(seedKey, m_onlyRoI, m_etaWindow, m_phiWindow);
      if( !cnv.serialize( key, ctx, tree ) ){
        MSG_FATAL("Its not possible to serialize xAOD::CaloHitContainer.");
      };
    }else{
      MSG_WARNING("There is not converver for the container " << container << " in key " << key);
    }
  }

  return StatusCode::SUCCESS;
 
}
                           
//!=====================================================================







