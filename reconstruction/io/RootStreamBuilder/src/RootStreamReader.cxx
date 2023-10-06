

#include "CaloHit/CaloHitConverter.h"
#include "CaloCell/CaloCellConverter.h"
#include "EventInfo/EventInfoConverter.h"
#include "EventInfo/EventSeedConverter.h"
#include "TruthParticle/TruthParticleConverter.h"
#include "RootStreamReader.h"
#include "TTree.h"

#include <boost/algorithm/string.hpp>


using namespace SG;
using namespace Gaugi;
using namespace xAOD;


RootStreamReader::RootStreamReader( std::string name ) : 
  IMsgService(name),
  Algorithm()
{
  declareProperty( "OutputLevel"             , m_outputLevel=1                   );
  declareProperty( "InputFile"               , m_inputFile=""                    );
  declareProperty( "NtupleName"              , m_ntupleName="CollectionTree"     );
}

//!=====================================================================

RootStreamReader::~RootStreamReader()
{;}

//!=====================================================================

StatusCode RootStreamReader::initialize()
{
  CHECK_INIT();
  setMsgLevel(m_outputLevel);
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamReader::bookHistograms( SG::EventContext &ctx ) const
{
  MSG_INFO("Reading file " << m_inputFile);
  auto store = ctx.getStoreGateSvc();
  TFile *file = new TFile(m_inputFile.c_str(), "read");
  store->decorate( "events", file );
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamReader::pre_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamReader::execute( EventContext &/*ctx*/, const G4Step * /*step*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamReader::execute( EventContext &ctx, int evt ) const
{
  return deserialize(evt, ctx);
}

//!=====================================================================

StatusCode RootStreamReader::finalize()
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamReader::post_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamReader::fillHistograms( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamReader::deserialize( int evt, EventContext &ctx ) const
{
  MSG_INFO("Deserialize all containers...");

  auto store = ctx.getStoreGateSvc();
  TFile *file = (TFile*)store->decorator("events");
  TTree *tree = (TTree*)file->Get(m_ntupleName.c_str());



  std::vector<std::string> containers;
  auto branches = tree->GetListOfBranches();
  for( auto branch : *branches)
  {
    auto name = branch->GetName();
    MSG_INFO(name);
    containers.push_back(name);
  }

  std::string seedKey = "Seeds";


  for (auto &container_key : containers){
    
    MSG_INFO("Preparing " << container_key << "...");

    std::vector<std::string> result;
    boost::split(result, container_key, boost::is_any_of("_") );
    auto container = result.at(0);
    auto key       = result.at(1);

    if (container == "EventInfoContainer"){

      MSG_INFO("Converting and deseriallizing EventInfo objects into root file...");
      EventInfoConverter cnv;
      if( !cnv.deserialize( key, evt, tree, ctx ) ){
        MSG_FATAL("Its not possible to deserialize xAOD::EventInfoContainer.");
      }

    }else if (container == "EventSeedContainer"){

      MSG_INFO("Converting and deseriallizing EventSeed objects into root file...");
      EventSeedConverter cnv;
      if( !cnv.deserialize( key, evt, tree, ctx ) ){
        MSG_FATAL("Its not possible to deserialize xAOD::EventSeedContainer.");
      }
      seedKey = key;

    }else if (container == "TruthParticleContainer"){

      MSG_INFO("Converting and deseriallizing TruthParticle objects into root file...");
      TruthParticleConverter cnv;
      if( !cnv.deserialize( key, evt, tree, ctx ) ){
        MSG_FATAL("Its not possible to deserialize xAOD::TruthParticleContainer.");
      }

    }else if (container == "CaloCellContainer"){

      MSG_INFO("Converting and deseriallizing CaloCell objects into root file...");
      CaloCellConverter cnv(seedKey , 0.4, 0.4 );
      if( !cnv.deserialize( key, evt, tree, ctx ) ){
        MSG_FATAL("Its not possible to deserialize xAOD::CaloCellContainer.");
      }

    }else if (container == "CaloHitContainer"){

      MSG_INFO("Converting and deseriallizing CaloHit objects into root file...");
      CaloHitConverter cnv(seedKey, false, 0.4, 0.4);
      if( !cnv.deserialize( key, evt, tree, ctx ) ){
        MSG_FATAL("Its not possible to deserialize xAOD::CaloHitContainer.");
      }
    }
  }

  return StatusCode::SUCCESS;
 
}
                           
//!=====================================================================







