#ifndef EventLoop_h 
#define EventLoop_h

/** simulator includes **/
#include "core/StoreGate.h"
#include "core/EventContext.h"
#include "core/IAlgTool.h"

/** geant 4 includes **/
#include "G4Run.hh"
#include "globals.hh"
#include "G4Step.hh"

/** standard includes **/
#include <string>
#include <vector>


class EventLoop : public G4Run
{
  public:

    /** Constructor **/
    EventLoop();
    /** Destructor **/
    virtual ~EventLoop();
    
    /** Used in Geant core, Loop event **/

    /** Pre execution used in event action step **/
    void BeginOfEvent();
    /** execute a step point to run data. used in step action **/
    void ExecuteEvent(const G4Step *step);
    /** Pos execution used in event action step **/
    void EndOfEvent();

    /** Get the context. To be used externally in Geant core **/
    EventContext* getContext();


  private:

    // the event context
    EventContext *m_context;
    // the monitoring tool
    SG::StoreGate *m_store;
    // list of alg tools to be executed in loop
    std::vector < IAlgTool* > m_toolHandles;

};

  

#endif

