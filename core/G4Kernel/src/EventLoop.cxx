
#include "G4Kernel/constants.h"
#include "G4Kernel/EventLoop.h"
#include "GaugiKernel/Timer.h"
#include "G4Threading.hh"
#include "G4SystemOfUnits.hh"
#include <iostream>
#include <time.h>

  //m_store( output , G4Threading::G4GetThreadId() ),

EventLoop::EventLoop( std::vector<Gaugi::Algorithm*> acc , std::string output): 
  IMsgService("EventLoop"),
  G4Run(), 
  m_ctx( "EventContext" ),
  m_toolHandles(acc),
  m_lock(false)
{
  // Tranfer all rights to the event context
  //m_ctx.setStoreGateSvc( &m_store );

  //bookHistograms();

  // Pre execution of all tools in sequence
  for( auto &toolHandle : m_toolHandles){
    MSG_INFO( "Booking histograms for " << toolHandle->name() );
    //if (toolHandle->bookHistograms( m_ctx ).isFailure() ){
    //  MSG_FATAL("It's not possible to book histograms for " << toolHandle->name());
    //}
  }
}


EventLoop::~EventLoop()
{}


void EventLoop::BeginOfEvent()
{
  MSG_INFO("EventLoop::BeginOfEvent...");
  
  m_stepCounter=0;
  
  Gaugi::Timer timer;
  m_timeout.start();
  timer.start();

  // Unlock the execution
  unlock();
  
  // Pre execution of all tools in sequence
  if(!m_lock){
    //m_store.cd("Event");
    //m_store.histI("EventCounter")->Fill("Event",1);
    for( auto &toolHandle : m_toolHandles){
      MSG_INFO( "Launching pre execute step for " << toolHandle->name() );
      if (toolHandle->pre_execute( m_ctx ).isFailure() ){
        MSG_FATAL("It's not possible to pre execute " << toolHandle->name());
      }
    }
  }

  timer.stop();
  //m_store.cd("Event");
  //m_store.hist1( "BeginOfEvent" )->Fill( timer.resume() );
}


void EventLoop::ExecuteEvent( const G4Step* step )
{
  m_stepCounter++;
  float edep = (float)step->GetTotalEnergyDeposit()/MeV;

  if(edep>0){
    if(!m_lock){
      for( auto &toolHandle : m_toolHandles){
        if (toolHandle->execute( m_ctx, step ).isFailure() ){
          MSG_FATAL("Execution failure for  " << toolHandle->name());
        }
      }
    }
  }
  
}


void EventLoop::EndOfEvent()
{

  
  Gaugi::Timer timer;
  
  timer.start();
  if (!m_lock){
    MSG_INFO("EventLoop::EndOfEvent...");
    for( auto &toolHandle : m_toolHandles){
      MSG_INFO( "Launching post execute step for " << toolHandle->name() );
      if (toolHandle->post_execute( m_ctx ).isFailure() ){
        MSG_FATAL("It's not possible to post execute for " << toolHandle->name());
      }
      //if (toolHandle->fillHistograms( m_ctx ).isFailure() ){
      //  MSG_FATAL("It's not possible to fill histograms for " << toolHandle->name());
      //}
    }
    //m_store.cd("Event");
    //m_store.histI("EventCounter")->Fill("Completed",1);
  }else{
    //m_store.cd("Event");
    //m_store.histI("EventCounter")->Fill("Timeout",1);
  }

  // Clear all storable pointers
  m_ctx.clear();


  timer.stop();
  m_timeout.stop();  

  //m_store.cd("Event");
  //m_store.hist1( "EndOfEvent" )->Fill( timer.resume() );
  //m_store.hist1( "Event" )->Fill( m_timeout.resume() );

  MSG_INFO( "Event loop was completed with " << m_stepCounter << " G4Steps and " << m_timeout.resume() << " seconds." );
}


void EventLoop::bookHistograms(){

  /*
  m_store.cd();
  m_store.mkdir( "Event" );
  m_store.add( new TH1F("BeginOfEvent" , ";time[s];Count;"   , 100 , 0 , 1) ) ;
  m_store.add( new TH1F("EndOfEvent"   , ";time[s];Count;"   , 100 , 0 , 10 ) );
  m_store.add( new TH1F("Event"        , ";time[s];Count;"   , 600 , 0 , 600) );
  m_store.add( new TH1I("EventCounter" , ";;Count;"           , 3  , 0 ,   3) );
  
  std::vector<std::string> labels{"Event", "Completed", "Timeout"};
  m_store.setLabels( m_store.histI("EventCounter"), labels );
  */
  
} 




SG::EventContext & EventLoop::getContext()
{
  return m_ctx;
}

bool EventLoop::timeout(){
  return m_timeout.resume() > event_timeout ? true : false;
}

void EventLoop::lock(){
  m_lock=true;
}

void EventLoop::unlock(){
  m_lock=false;
}



