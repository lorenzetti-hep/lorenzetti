
#include "core/EventLoop.h"
#include "core/EventContext.h"
#include "core/StoreGate.h"
#include "EventInfo/EventInfo.h"


EventLoop::EventLoop( std::vector<IAlgTool*> sequence, std::string output) : 
  G4Run(),
  m_toolHandles(sequence),
  m_output(output),
  m_outputStepKey("G4_Step")
{
  initialize();
}


EventLoop::~EventLoop()
{
  MSG_INFO("Destroy event loop...");
  finalize();
}


void EventLoop::initialize(){

  // Create the event context  
  m_context = new EventContext();

  // Create the monitoring tool
  m_store = new SG::StoreGate(m_output);


  // Lint the event context, monitoring for each reco alg (tool)
  for( auto &toolHandle : m_toolHandles ){
    MSG_INFO( "Initialize " << toolHandle->name() );
    toolHandle->setStoreGateSvc( getStoreGateSvc() );
    toolHandle->setContext( getContext() );
    toolHandle->initialize();
  }

}


void EventLoop::finalize(){
  
  delete m_store;
  //delete m_context;
}


void EventLoop::BeginOfEvent()
{

  getContext()->initialize(); 

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
  getContext()->attach( step, m_outputStepKey, true );

  // Pre execution of all tools in sequence
  for( auto &toolHandle : m_toolHandles){
  
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

    toolHandle->fillHistograms( getContext() );
  }

  // Release all memory allocated for all containers during the 
  // tool handle execution
  getContext()->finalize();
  MSG_INFO("EndOfEvent::done");
}


SG::StoreGate* EventLoop::getStoreGateSvc()
{
  return m_store;
}

EventContext* EventLoop::getContext()
{
  return m_context;
}











