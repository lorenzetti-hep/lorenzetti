
#include "G4Kernel/constants.h"
#include "G4Kernel/EventLoop.h"
#include "G4Threading.hh"
#include <iostream>
#include <time.h>


EventLoop::EventLoop( std::vector<Gaugi::Algorithm*> acc , std::string output): 
  IMsgService("EventLoop"),
  G4Run(), 
  m_store( output , G4Threading::G4GetThreadId() ),
  m_ctx( "EventContext" ),
  m_toolHandles(acc),
  m_nEvents(0),
  m_nGoodEvents(0),
  m_lock(false)
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
{

  unsigned badEvents = m_nEvents - m_nGoodEvents;
  MSG_INFO( "Good events    : " << m_nGoodEvents );
  MSG_INFO( "Aborted events : " << badEvents );
}




void EventLoop::BeginOfEvent()
{
  MSG_INFO("EventLoop::BeginOfEvent...");
  
  m_nEvents++;

  // Unlock the execution
  unlock();
  
  // start the event counter
  start();


  // Pre execution of all tools in sequence
  if(!m_lock){
    for( auto &toolHandle : m_toolHandles){
      MSG_INFO( "Launching pre execute step for " << toolHandle->name() );
      if (toolHandle->pre_execute( m_ctx ).isFailure() ){
        MSG_FATAL("It's not possible to pre execute " << toolHandle->name());
      }
    }
  }
}


void EventLoop::ExecuteEvent( const G4Step* step )
{
  update();
  if(!m_lock){
    for( auto &toolHandle : m_toolHandles){
      if (toolHandle->execute( m_ctx, step ).isFailure() ){
        MSG_FATAL("Execution failure for  " << toolHandle->name());
      }
    }
  }
}


void EventLoop::EndOfEvent()
{
  if (!m_lock){
    MSG_INFO("EventLoop::EndOfEvent...");
    for( auto &toolHandle : m_toolHandles){
      MSG_INFO( "Launching post execute step for " << toolHandle->name() );
      if (toolHandle->post_execute( m_ctx ).isFailure() ){
        MSG_FATAL("It's not possible to post execute for " << toolHandle->name());
      }
      if (toolHandle->fillHistograms( m_ctx , m_store).isFailure() ){
        MSG_FATAL("It's not possible to fill histograms for " << toolHandle->name());
      }
    }
  }


  // Clear all storable pointers
  m_ctx.clear();
  update();

  m_nGoodEvents++;
  MSG_INFO( "Event loop take " << (m_end-m_start) << " seconds to be processed." );
}


SG::EventContext & EventLoop::getContext()
{
  return m_ctx;
}



bool EventLoop::timeout(){
  return (m_end-m_start) > event_timeout ? true : false;
}



void EventLoop::start(){
  m_start = m_end = time(nullptr);
}


void EventLoop::update(){
  m_end = time(nullptr);
}

void EventLoop::lock(){
  m_lock=true;
}

void EventLoop::unlock(){
  m_lock=false;
}



