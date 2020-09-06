
#include "G4Kernel/EventLoop.h"
#include "G4Kernel/EventAction.h"


#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"
#include <iomanip>


EventAction::EventAction()
 : IMsgService("EventAction"), 
   G4UserEventAction()
{;}


EventAction::~EventAction()
{;}


void EventAction::BeginOfEventAction(const G4Event* /*event*/)
{  
  EventLoop* loop = static_cast<EventLoop*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  MSG_DEBUG( "EventAction::BeginOfEvent()" );
  loop->BeginOfEvent();
}



void EventAction::EndOfEventAction(const G4Event* /*event*/)
{
  MSG_DEBUG( "EventAction::EndOfEvent()" );
  EventLoop* loop = static_cast<EventLoop*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  loop->EndOfEvent();
}




