
#include "G4Kernel/EventLoop.h"
#include <iostream>


EventLoop::EventLoop( std::vector<Gaugi::Algorithm*> acc ): 
  IMsgService("EventLoop"),
  G4Run(), 
  m_ctx( "EventContext" ),
  m_toolHandles(acc)

{}


EventLoop::~EventLoop()
{;}




void EventLoop::BeginOfEvent()
{
  // Pre execution of all tools in sequence
  for( auto &toolHandle : m_toolHandles){
    MSG_INFO( "Launching pre execute step for " << toolHandle->name() );
    if (toolHandle->pre_execute( m_ctx ).isFailure() ){
      MSG_FATAL("It's not possible to pre execute " << toolHandle->name());
    }

  }
}


void EventLoop::ExecuteEvent( const G4Step* step )
{
  for( auto &toolHandle : m_toolHandles){
    if (toolHandle->execute( m_ctx, step ).isFailure() ){
      MSG_FATAL("Execution failure for  " << toolHandle->name());
    }
  }
}


void EventLoop::EndOfEvent()
{
  for( auto &toolHandle : m_toolHandles){

    MSG_INFO( "Launching post execute step for " << toolHandle->name() );
    
    if (toolHandle->post_execute( m_ctx ).isFailure() ){
      MSG_FATAL("It's not possible to post execute " << toolHandle->name());
    }

    toolHandle->fillHistograms( m_ctx );
  }

  // Clear all storable pointers
  m_ctx.clear();
}


SG::EventContext & EventLoop::getContext()
{
  return m_ctx;
}










