

#include "RootStreamMaker.h"
#include "TTree.h"



using namespace SG;
using namespace Gaugi;
using namespace xAOD;


RootStreamMaker::RootStreamMaker( std::string name ) : 
  IMsgService(name),
  Algorithm()
{
  declareProperty( "Converters"              , m_converters={}         );
  declareProperty( "OutputLevel"             , m_outputLevel=1         );
}

//!=====================================================================

RootStreamMaker::~RootStreamMaker()
{;}

//!=====================================================================

StatusCode RootStreamMaker::initialize()
{
  CHECK_INIT();
  //setMsgLevel(m_outputLevel);
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamMaker::bookHistograms( SG::EventContext &ctx ) const
{
  auto store = ctx.getStoreGateSvc();
  store->cd();
  store->mkdir("CollectionTree");
  for (auto &cnv : m_converters){
    std::string treename = cnv->container() + "_" + cnv->key();
    TTree *tree = new TTree((treename).c_str(), "");
    store->add( tree );
  }
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
  MSG_INFO("Serialize all containers...");

  auto store = ctx.getStoreGateSvc();
  store->cd("CollectionTree");

  for (auto &cnv : m_converters){
    
    MSG_INFO("Preparing " << container_key << "...");
    
    std::string treename = cnv->container() + "_" + cnv->key();
    TTree *tree = store->tree(treename);

    if ( cnv->serialize( ctx , tree ).isFailure() ){
      MSG_FATAL();
    }
  }
}

//!=====================================================================

   






