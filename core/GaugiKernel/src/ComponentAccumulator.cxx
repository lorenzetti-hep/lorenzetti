
#include "GaugiKernel/ComponentAccumulator.h"
#include <iostream>
#include <time.h>

using namespace SG;
using namespace Gaugi;

//!=====================================================================

ComponentAccumulator::ComponentAccumulator( std::string name 
                                            //std::string output,
                                            //int threadId,
                                            //int numberOfThreads
                                            //std::shared_ptr<ROOT::Experimental::TBufferMergerFile> file
                                            ): 
  IMsgService(name)
  //m_ctx( "EventContext" )
  //m_store( output, threadId )
{
  //m_store = new SG::StoreGate(output, threadId);
  //m_ctx.setStoreGateSvc( m_store );
  //m_ctx.setThreadId(threadId);
  //m_ctx.setNumberOfThreads(numberOfThreads);
}

//!=====================================================================

ComponentAccumulator::~ComponentAccumulator()
{
}

//!=====================================================================

void ComponentAccumulator::push_back( Gaugi::Algorithm* tool )
{
  m_toolHandles.push_back(tool);
}

//!=====================================================================

void ComponentAccumulator::initialize( )
{
  MSG_INFO("Initialize...");


  for ( auto toolHandle : m_toolHandles )
  { 
    MSG_INFO( "Initializing the tool with name " << toolHandle->name() );
    if ( toolHandle->initialize().isFailure() )
    {
      MSG_ERROR("It's not possible to initialize the tool with name: " << toolHandle->name() );
    }
 
  }


}

//!=====================================================================

void ComponentAccumulator::bookHistograms( SG::EventContext *ctx ) const
{


  MSG_INFO("Initialize...");

  auto store = ctx->getStoreGateSvc();

  for ( auto toolHandle : m_toolHandles )
  { 
    MSG_INFO( "Booking histograms for " << toolHandle->name() );
    if (toolHandle->bookHistograms( *ctx ).isFailure() ){
      MSG_FATAL("It's not possible to book histograms for " << toolHandle->name());
    }
  }

  store->cd();
  store->mkdir( "Event" );
  store->add( new TH1F("Event"        , ";time[s];Count;"   , 600 , 0 , 600) );
  store->add( new TH1I("EventCounter" , ";;Count;"           , 3  , 0 ,   3) );
  std::vector<std::string> labels{"Event", "Completed"};
  store->setLabels( store->histI("EventCounter"), labels );
}


//!=====================================================================

void ComponentAccumulator::finalize()
{
  for ( auto toolHandle : m_toolHandles )
  { 
    if ( toolHandle->finalize().isFailure() )
    {
      MSG_ERROR("It's not possible to finalize the tool with name: " << toolHandle->name() );
    }
  }
}

//!=====================================================================

void ComponentAccumulator::run(SG::EventContext *ctx , int evt) const
{
  MSG_INFO("======================= Event "<< evt << "=========================");
  Timer timer;

  timer.start();
  ctx->clear();

  auto store = ctx->getStoreGateSvc();

  for( auto &toolHandle : m_toolHandles){
    
    //MSG_INFO( "Launching execute step for " << toolHandle->name() );
    if (toolHandle->execute( *ctx , evt ).isFailure() ){
      MSG_FATAL("It's not possible to execute for " << toolHandle->name());
    }
    
    //MSG_INFO( "Launching booking step for " << toolHandle->name() );
    if (toolHandle->fillHistograms( *ctx ).isFailure() ){
      MSG_FATAL("It's not possible to fill histograms for " << toolHandle->name());
    }
  }
  store->cd("Event");
  store->histI("EventCounter")->Fill("Completed",1);
  timer.stop();
  store->cd("Event");
  store->hist1( "Event" )->Fill( timer.resume() );

  MSG_INFO("===================================================================");
}






