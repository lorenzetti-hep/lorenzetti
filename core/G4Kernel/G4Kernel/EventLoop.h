#ifndef EventLoop_h 
#define EventLoop_h

#include "GaugiKernel/DataHandle.h"
#include "GaugiKernel/MsgStream.h"
#include "GaugiKernel/Algorithm.h"
#include "GaugiKernel/StoreGate.h"
#include "G4Run.hh"
#include "globals.hh"
#include "G4Step.hh"
#include <string>
#include <vector>


class EventLoop : public G4Run, public MsgService
{
  public:

    /** Constructor **/
    EventLoop( std::vector<Gaugi::Algorithm*>, std::string output );
    
    /** Destructor **/
    virtual ~EventLoop();
    
    /** Used in Geant core, Loop event **/

    /** Pre execution used in event action step **/
    void BeginOfEvent();
    /** execute a step point to run data. used in step action **/
    void ExecuteEvent(const G4Step *step);
    /** Pos execution used in event action step **/
    void EndOfEvent();


    SG::EventContext& getContext();


  private:

    // Store gate
    SG::StoreGate m_store;

    // the event context
    SG::EventContext m_ctx;
    
    // list of alg tools to be executed in loop
    std::vector < Gaugi::Algorithm* > m_toolHandles;
};

  

#endif

