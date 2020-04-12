#ifndef SteppingAction_h
#define SteppingAction_h

#include "GaugiKernel/MsgStream.h"
#include "G4UserSteppingAction.hh"
#include "g4root.hh"

class DetectorConstruction;

class SteppingAction : public G4UserSteppingAction, public MsgService
{
  public:
    SteppingAction();
    virtual ~SteppingAction();
    virtual void UserSteppingAction(const G4Step* step);
  
};


#endif
