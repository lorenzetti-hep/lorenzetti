
#include "core/EventLoop.h"
#include "core/geant/EventAction.h"


#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"
#include <iomanip>


EventAction::EventAction()
 : G4UserEventAction()
{;}


EventAction::~EventAction()
{;}


void EventAction::BeginOfEventAction(const G4Event* /*event*/)
{  
  EventLoop* loop = static_cast<EventLoop*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  loop->BeginOfEvent();
}



void EventAction::EndOfEventAction(const G4Event* event)
{
  G4PrimaryVertex* primaryVertex = event->GetPrimaryVertex();
  G4PrimaryParticle* primaryParticle = primaryVertex->GetPrimary();
  G4double ke = primaryParticle->GetKineticEnergy()/1000.; //in GeV.
  //runData->GetEventInfo()->setTotalEnergy(ke);
  EventLoop* loop = static_cast<EventLoop*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  
  loop->EndOfEvent();
}




