
#include "G4Kernel/EventLoop.h"
#include "G4Kernel/SteppingAction.h"

#include "G4Step.hh"
#include "G4RunManager.hh"


SteppingAction::SteppingAction()
  : G4UserSteppingAction()
{;}


SteppingAction::~SteppingAction()
{;}



void SteppingAction::UserSteppingAction(const G4Step* step)
{
  EventLoop* loop = static_cast<EventLoop*> (G4RunManager::GetRunManager()->GetNonConstCurrentRun()); 
  loop->ExecuteEvent(step); 
}






