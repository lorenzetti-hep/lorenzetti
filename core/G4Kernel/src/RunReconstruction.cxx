
#include "G4Kernel/constants.h"
#include "G4Kernel/RunReconstruction.h"
#include "GaugiKernel/Timer.h"
#include "G4Threading.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include <string>
#include <iostream>
#include <time.h>



RunReconstruction::RunReconstruction( int numberOfThreads,
                                      std::vector<Gaugi::Algorithm*> acc , 
                                      std::string output ): 
  IMsgService("RunReconstruction"),
  G4Run(), 
  m_store( output + "." + std::to_string(G4Threading::G4GetThreadId()) ),
  m_ctx( "EventContext" ),
  m_toolHandles(acc),
  m_lock(false)
{
  // Tranfer all rights to the event context
  m_ctx.setStoreGateSvc( &m_store );
  m_ctx.setNumberOfThreads(numberOfThreads);
  m_ctx.setThreadId(G4Threading::G4GetThreadId());
  bookHistograms();

  // Pre execution of all tools in sequence
  for( auto &toolHandle : m_toolHandles){
    MSG_DEBUG( "Booking histograms for " << toolHandle->name() );
    if (toolHandle->bookHistograms( m_ctx ).isFailure() ){
      MSG_FATAL("It's not possible to book histograms for " << toolHandle->name());
    }
  }
}

//!=====================================================================

RunReconstruction::~RunReconstruction()
{
  m_store.save();
}

//!=====================================================================

void RunReconstruction::BeginOfEvent()
{
  MSG_INFO("RunReconstruction::BeginOfEvent...");
  
  m_stepCounter=0;
  
  Gaugi::Timer timer;
  m_timeout.start();
  timer.start();

  // Unlock the execution
  unlock();
  
  // Pre execution of all tools in sequence
  if(!m_lock){
    m_store.cd("Event");
    m_store.histI("EventCounter")->Fill("Event",1);
    for( auto &toolHandle : m_toolHandles){
      MSG_DEBUG( "Launching pre execute step for " << toolHandle->name() );
      if (toolHandle->pre_execute( m_ctx ).isFailure() ){
        MSG_FATAL("It's not possible to pre execute " << toolHandle->name());
      }
    }
  }

  timer.stop();
  m_store.cd("Event");
  m_store.hist1( "BeginOfEvent" )->Fill( timer.resume() );
  m_msgCounter=0;
}

//!=====================================================================

void RunReconstruction::ExecuteEvent( const G4Step* step )
{
  m_stepCounter++;
  float edep = (float)step->GetTotalEnergyDeposit()/MeV;

  if ( m_msgCounter>1e6 && m_timeout.resume() > event_timeout/2 ){
    MSG_INFO( "Running... " << m_timeout.resume() << " seconds (timeout: "  << event_timeout << " seconds)" );
    m_msgCounter=0;
  }

  if(edep>0)
  {
    if(!m_lock){
      for( auto &toolHandle : m_toolHandles){
        if (toolHandle->execute( m_ctx, step ).isFailure() ){
          MSG_FATAL("Execution failure for  " << toolHandle->name());
        }
      }
    }
  }
  
  m_msgCounter++;
}

//!=====================================================================

void RunReconstruction::EndOfEvent()
{

  
  Gaugi::Timer timer;
  
  timer.start();
  if (!m_lock){
    MSG_INFO("RunReconstruction::EndOfEvent...");
    for( auto &toolHandle : m_toolHandles){
      MSG_DEBUG( "Launching post execute step for " << toolHandle->name() );
      if (toolHandle->post_execute( m_ctx ).isFailure() ){
        MSG_FATAL("It's not possible to post execute for " << toolHandle->name());
      }
      if (toolHandle->fillHistograms( m_ctx ).isFailure() ){
        MSG_FATAL("It's not possible to fill histograms for " << toolHandle->name());
      }
    }
    m_store.cd("Event");
    m_store.histI("EventCounter")->Fill("Completed",1);
  }else{
    m_store.cd("Event");
    m_store.histI("EventCounter")->Fill("Timeout",1);
  }

  // Clear all storable pointers
  m_ctx.clear();


  timer.stop();
  m_timeout.stop();  

  m_store.cd("Event");
  m_store.hist1( "EndOfEvent" )->Fill( timer.resume() );
  m_store.hist1( "Event" )->Fill( m_timeout.resume() );

  MSG_INFO( "Event loop was completed with " << m_stepCounter << " G4Steps and " << m_timeout.resume() << " seconds." );
}

//!=====================================================================

void RunReconstruction::bookHistograms(){

  m_store.cd();
  m_store.mkdir( "Event" );
  m_store.add( new TH1F("BeginOfEvent" , ";time[s];Count;"   , 100 , 0 , 1  ) );
  m_store.add( new TH1F("EndOfEvent"   , ";time[s];Count;"   , 100 , 0 , 10 ) );
  m_store.add( new TH1F("Event"        , ";time[s];Count;"   , 600 , 0 , 600) );
  m_store.add( new TH1I("EventCounter" , ";;Count;"           , 3  , 0 ,   3) );
  
  std::vector<std::string> labels{"Event", "Completed", "Timeout"};
  m_store.setLabels( m_store.histI("EventCounter"), labels );
  
} 

//!=====================================================================

SG::EventContext & RunReconstruction::getContext()
{
  return m_ctx;
}

//!=====================================================================

bool RunReconstruction::timeout(){
  return m_timeout.resume() > event_timeout ? true : false;
}

//!=====================================================================

void RunReconstruction::lock(){
  m_lock=true;
}

//!=====================================================================

void RunReconstruction::unlock(){
  m_lock=false;
}

//!=====================================================================


