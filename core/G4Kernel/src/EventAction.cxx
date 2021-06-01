
#include "G4Kernel/RunReconstruction.h"
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
  RunReconstruction* reco = static_cast<RunReconstruction*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  MSG_DEBUG( "EventAction::BeginOfEvent()" );
  reco->BeginOfEvent();
}



void EventAction::EndOfEventAction(const G4Event* /*event*/)
{
  MSG_DEBUG( "EventAction::EndOfEvent()" );
  RunReconstruction* reco = static_cast<RunReconstruction*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  reco->EndOfEvent();
}




