
#include "GaugiKernel/ComponentAccumulator.h"
#include <iostream>
#include <time.h>

using namespace SG;
using namespace Gaugi;

ComponentAccumulator::ComponentAccumulator( std::string name , std::string output ): 
  IMsgService(name),
  m_store( output , 0 ),
  m_ctx( "EventContext" )
{
  // Tranfer all rights to the event context
  m_ctx.setStoreGateSvc( &m_store );
}


ComponentAccumulator::~ComponentAccumulator()
{}


void ComponentAccumulator::push_back( Gaugi::Algorithm* tool )
{
  m_toolHandles.push_back(tool);
}

void ComponentAccumulator::setReader( Gaugi::ComponentReader* reader )
{
  m_reader=reader;
}

void ComponentAccumulator::initialize()
{

  if(!m_reader){
    MSG_FATAL("You must merge some reader into the main component.");
  }

  if(m_reader->initialize().isFailure()){
    MSG_FATAL("Not possible to initialize the event reader component: " << m_reader->name());
  }

  for ( auto toolHandle : m_toolHandles )
  { 
    MSG_INFO( "Initializing the tool with name " << toolHandle->name() );
    if ( toolHandle->initialize().isFailure() )
    {
      MSG_ERROR("It's not possible to initialize the tool with name: " << toolHandle->name() );
    }
  } 

  bookHistograms();
}


void ComponentAccumulator::finalize()
{
  for ( auto toolHandle : m_toolHandles )
  { 
    if ( toolHandle->finalize().isFailure() )
    {
      MSG_ERROR("It's not possible to finalize the tool with name: " << toolHandle->name() );
    }
  }

  if(m_reader->finalize().isFailure())
  {
    MSG_ERROR("Not possible to finalize the event reader component: " << m_reader->name());
  }

}


void ComponentAccumulator::run(int nov)
{

  int completed = 0;
  for (int evt=0; evt < m_reader->GetEntries(); ++evt)
  {
    
    MSG_INFO("======================= Event "<< evt << "=========================");
    m_reader->GeneratePrimaryVertex(evt, m_ctx);
    BeginOfEvent();
    ExecuteEvent();
    EndOfEvent();
    MSG_INFO("===================================================================");

    if (nov >= 0 && completed > nov ){
      break; // force stop
    }
    completed++;

  }

}


void ComponentAccumulator::BeginOfEvent()
{
  MSG_INFO("ComponentAccumulator::BeginOfEvent...");
  Gaugi::Timer timer;
  m_timer.start();

  // Pre execution of all tools in sequence
  m_store.cd("Event");
  m_store.histI("EventCounter")->Fill("Event",1);
  for( auto &toolHandle : m_toolHandles){
    MSG_DEBUG( "Launching pre execute step for " << toolHandle->name() );
    if (toolHandle->pre_execute( m_ctx ).isFailure() ){
      MSG_FATAL("It's not possible to pre execute " << toolHandle->name());
    }
  }
}

void ComponentAccumulator::ExecuteEvent()
{
  // do nothing here since this is not geant
}



void ComponentAccumulator::EndOfEvent()
{
  MSG_INFO("ComponentAccumulator::EndOfEvent...");
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

  // Clear all storable pointers
  m_ctx.clear();
  m_timer.stop();
  m_store.cd("Event");
  m_store.hist1( "Event" )->Fill( m_timer.resume() );

}


void ComponentAccumulator::bookHistograms()
{
  m_store.cd();
  m_store.mkdir( "Event" );
  m_store.add( new TH1F("Event"        , ";time[s];Count;"   , 600 , 0 , 600) );
  m_store.add( new TH1I("EventCounter" , ";;Count;"           , 3  , 0 ,   3) );
  std::vector<std::string> labels{"Event", "Completed"};
  m_store.setLabels( m_store.histI("EventCounter"), labels );

  for( auto &toolHandle : m_toolHandles){
    MSG_DEBUG( "Booking histograms for " << toolHandle->name() );
    if (toolHandle->bookHistograms( m_ctx ).isFailure() ){
      MSG_FATAL("It's not possible to book histograms for " << toolHandle->name());
    }
  }
} 




