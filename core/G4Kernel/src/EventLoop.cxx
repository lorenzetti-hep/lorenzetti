
#include "G4Kernel/EventLoop.h"
#include "G4Threading.hh"
#include <iostream>



EventLoop::EventLoop( std::vector<Gaugi::Algorithm*> acc , std::string output): 
  IMsgService("EventLoop"),
  G4Run(), 
  m_store( output , G4Threading::G4GetThreadId() ),
  m_ctx( "EventContext" ),
  m_toolHandles(acc)

{
  // Pre execution of all tools in sequence
  for( auto &toolHandle : m_toolHandles){
    MSG_INFO( "Booking histograms for " << toolHandle->name() );
    if (toolHandle->bookHistograms( m_store ).isFailure() ){
      MSG_FATAL("It's not possible to book histograms for " << toolHandle->name());
    }
  }
}


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
      MSG_FATAL("It's not possible to post execute for " << toolHandle->name());
    }
    if (toolHandle->fillHistograms( m_ctx , m_store).isFailure() ){
      MSG_FATAL("It's not possible to fill histograms for " << toolHandle->name());
    }
  }

  // Clear all storable pointers
  m_ctx.clear();
}


SG::EventContext & EventLoop::getContext()
{
  return m_ctx;
}










