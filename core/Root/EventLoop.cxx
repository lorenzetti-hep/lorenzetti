
#include "core/EventLoop.h"
#include "core/EventContext.h"
#include "core/StoreGate.h"
//#include "CaloCell/CaloCellMaker.h"
//#include "CaloCluster/CaloClusterMaker.h"


EventLoop::EventLoop() : G4Run()
{
  initialize();
}


EventLoop::~EventLoop()
{
  finalize();
}


void EventLoop::initialize(){

  // Create the event context  
  m_context = new EventContext();
  //m_context->initialize();

  // Create the monitoring tool
  m_store = new SG::StoreGate();
  //m_store->initialize();

  // Create the reconstruction sequence
  //m_tools.push_back( new CaloCellMaker("CaloCellMaker")       ); 
  //m_tools.push_back( new CaloClusterMaker("CaloClusterMaker") );

  // Lint the event context, monitoring for each reco alg (tool)
  for( auto &toolHandle : m_toolHandles ){
    toolHandle->setStoreGateSvc( getStoreGateSvc() );
    toolHandle->setContext( getContext() );
    toolHandle->initialize();
  }

}


void EventLoop::finalize(){

  for( auto &toolHandle : m_toolHandles ){
    toolHandle->finalize();
    delete toolHandle;
  }
 
  //m_monTool->finalize();
  delete m_store;

  //m_context->finalize();
  delete m_context;
}


void EventLoop::BeginOfEvent()
{
  // Pre execution of all tools in sequence
  for( auto &toolHandle : m_toolHandles){
    MSG_INFO( "Execute pre-execute for " << toolHandle->name() );

    if (toolHandle->pre_execute( getContext() ).isFailure() ){
      MSG_FATAL("It's not possible to pre-execute " << toolHandle->name());
    }

  }
}


void EventLoop::ExecuteEvent( const G4Step* step )
{
  getContext()->attach( step );

  // Pre execution of all tools in sequence
  for( auto &toolHandle : m_toolHandles){
    MSG_INFO( "Execute execute for " << toolHandle->name() );
  
    if (toolHandle->execute( getContext() ).isFailure() ){
      MSG_FATAL("It's not possible to execute " << toolHandle->name());
    }


  }
}


void EventLoop::EndOfEvent()
{
  // Pre execution of all tools in sequence
  for( auto &toolHandle : m_toolHandles){
    MSG_INFO( "Execute post-execute for " << toolHandle->name() );
    
    if (toolHandle->post_execute( getContext() ).isFailure() ){
      MSG_FATAL("It's not possible to post-execute " << toolHandle->name());
    }

    toolHandle->fill( getContext() );
  }
}


