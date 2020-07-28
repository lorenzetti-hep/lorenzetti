
#include "G4Kernel/EventLoop.h"
#include "G4Kernel/SteppingAction.h"

#include "G4Step.hh"
#include "G4RunManager.hh"


SteppingAction::SteppingAction()
  : IMsgService("SteppingAction"),
    G4UserSteppingAction()
{;}


SteppingAction::~SteppingAction()
{;}



void SteppingAction::UserSteppingAction(const G4Step* step)
{
  EventLoop* loop = static_cast<EventLoop*> (G4RunManager::GetRunManager()->GetNonConstCurrentRun()); 
  loop->ExecuteEvent(step);
  
  if (loop->timeout())
  {
    loop->lock(); // Force skip the post execute step
    std::string msg = "Event timeout!";
    G4Exception("EventLoop::UserSteppingAction()", "WatchDog", EventMustBeAborted, msg.c_str());
  }  
}






