
#include "G4Kernel/RunReconstruction.h"
#include "G4Kernel/SteppingAction.h"

#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

SteppingAction::SteppingAction()
  : IMsgService("SteppingAction"),
    G4UserSteppingAction()
{;}

//!=====================================================================

SteppingAction::~SteppingAction()
{;}

//!=====================================================================

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  RunReconstruction* reco = static_cast<RunReconstruction*> (G4RunManager::GetRunManager()->GetNonConstCurrentRun()); 
  reco->ExecuteEvent(step);
  
  if (reco->timeout())
  {
    reco->lock(); // Force skip the post execute step
    std::string msg = "Event timeout!";
    G4Exception("RunReconstruction::UserSteppingAction()", "WatchDog", EventMustBeAborted, msg.c_str());
  }  
}

//!=====================================================================






