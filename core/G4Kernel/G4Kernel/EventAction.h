#ifndef EventAction_h
#define EventAction_h

#include "GaugiKernel/MsgStream.h"
#include "G4UserEventAction.hh"
#include "globals.hh"


class EventAction : public G4UserEventAction, public MsgService
{
  public:
    EventAction();
    virtual ~EventAction();

    virtual void  BeginOfEventAction(const G4Event* event);
    virtual void    EndOfEventAction(const G4Event* event);
    
};
                     

#endif

    
