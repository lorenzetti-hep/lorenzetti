#ifndef RunReconstruction_h 
#define RunReconstruction_h

#include "GaugiKernel/DataHandle.h"
#include "GaugiKernel/MsgStream.h"
#include "GaugiKernel/Algorithm.h"
#include "GaugiKernel/StoreGate.h"
#include "GaugiKernel/Timer.h"
#include "G4Run.hh"
#include "globals.hh"
#include "G4Step.hh"
#include <string>
#include <vector>

class RunReconstruction : public G4Run, public MsgService
{
  public:

    /** Constructor **/
    RunReconstruction( int numberOfThreads, int timeout, std::vector<Gaugi::Algorithm*>, std::string output );
    
    /** Destructor **/
    virtual ~RunReconstruction();
    
    /** Used in Geant core, Loop event **/

    /** Pre execution used in event action step **/
    void BeginOfEvent();
    /** execute a step point to run data. used in step action **/
    void ExecuteEvent(const G4Step *step);
    /** Pos execution used in event action step **/
    void EndOfEvent();


    SG::EventContext& getContext();

    /** Is timeout? **/
    bool timeout();


    void lock();
    void unlock();

  private:

    void bookHistograms();

    // Store gate
    SG::StoreGate m_store;

    // the event context
    SG::EventContext m_ctx;
    
    // list of alg tools to be executed in loop
    std::vector < Gaugi::Algorithm* > m_toolHandles;

    // lock all steps
    bool m_lock;

    Gaugi::Timer m_timeout;

    unsigned m_stepCounter, m_msgCounter;

    int m_event_timeout;
};

  

#endif

