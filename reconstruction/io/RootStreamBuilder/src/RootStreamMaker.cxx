

#include "CaloHit/CaloHitConverter.h"
#include "EventInfo/EventInfoConverter.h"
#include "EventInfo/EventSeedConverter.h"
#include "TruthParticle/TruthParticleConverter.h"
#include "TruthParticle/ParticleSeedConverter.h"
#include "RootStreamMaker.h"
#include "GaugiKernel/EDM.h"
#include "G4Kernel/CaloPhiRange.h"
#include "TTree.h"

using namespace SG;
using namespace Gaugi;



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

StatusCode RootStreamMaker::execute( EventContext &ctx, int /*evt*/ ) const
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
 
  EventSeedConverter seedCnv;
  TruthParticleConverter truthCnv;
  CaloCellConverter cellCnv;
  CaloHitConverter hitCnv;

  for (auto &key : m_containers){
    container = 
    
    if (container == "EventInfoContainer"){
      EventInfoConverter cnv;
      cnv.serialize( ctx, key, tree );
      converters.push_back(cnv);
    }else if (container == "EventSeedContainer"){
      seedCnv.serialize( ctx, key, tree);
    }else if (container == "TruthParticleContainer"){
      truthCnv.serialize( ctx, key, tree);
    }else if (container == "CaloCellContainer"){
      cellCnv.serialize( ctx, key, tree);
    }else if (container == "CaloHitContainer"){
      hitCnv.serialize( ctx, key, tree);
    }
  }

  tree->Fill();
  return StatusCode::SUCCESS;
 
}

//!=====================================================================

