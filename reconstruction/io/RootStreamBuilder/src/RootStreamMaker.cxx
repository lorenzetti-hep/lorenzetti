

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

  MSG_INFO("Serialize...");
  auto store = ctx.getStoreGateSvc();

  for( auto key : ctx.keys())
  {
    MSG_INFO(key);
  }

  store->cd();
  TTree *tree = store->tree(m_ntupleName);
 
  // NOTE: this is unique always. We can not have more than one container for these objects stored into the contenx.
  EventInfoConverter                  eventCnv;
  EventSeedConverter                  seedCnv;
  std::vector<TruthParticleConverter> truthCnv;
  std::vector<CaloCellConverter>      cellCnv;
  std::vector<CaloHitConverter>       hitCnv;

  for (auto &container_key : m_containers){
    
    MSG_INFO("Storing " << container_key << "...");

    std::vector<std::string> result;
    boost::split(result, container_key, boost::is_any_of("_") );
    auto container = result.at(0);
    auto key = result.at(1);
    
    if (container == "EventInfoContainer"){

      MSG_INFO("Converting and serializing EventInfo objects into root file...");
      eventCnv.serialize( key, ctx, tree );

    }else if (container == "EventSeedContainer"){

      MSG_INFO("Converting and serializing EventSeed objects into root file...");
      seedCnv.serialize( key, ctx, tree );

    }else if (container == "TruthParticleContainer"){

      MSG_INFO("Converting and serializing TruthParticle objects into root file...");
      TruthParticleConverter cnv;
      cnv.serialize( key, ctx, tree );
      truthCnv.push_back(cnv);

    }else if (container == "CaloCellContainer"){

      MSG_INFO("Converting and serializing CaloCell objects into root file...");
      CaloCellConverter cnv(seedCnv.key() , m_etaWindow, m_phiWindow );
      cnv.serialize( key, ctx, tree );
      cellCnv.push_back(cnv);

    }else if (container == "CaloHitContainer"){

      MSG_INFO("Converting and serializing CaloHit objects into root file...");
      CaloHitConverter cnv(seedCnv.key(), m_onlyRoI, m_etaWindow, m_phiWindow);
      cnv.serialize( key, ctx, tree );
      hitCnv.push_back(cnv);
    }
  }

  tree->Fill();
  return StatusCode::SUCCESS;
 
}
                           
//!=====================================================================

