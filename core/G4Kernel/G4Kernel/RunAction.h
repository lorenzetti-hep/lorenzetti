#ifndef RunAction_h
#define RunAction_h

#include "GaugiKernel/MsgStream.h"
#include "GaugiKernel/Algorithm.h"

/** geant 4 includes **/
#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

class RunAction : public G4UserRunAction, public MsgService
{
  public:
    RunAction( int numberOfThreads, int timeout, std::vector<Gaugi::Algorithm*>, std::string output);
    virtual ~RunAction();
    virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);


  private:

    std::vector<Gaugi::Algorithm*> m_acc;
    std::string m_output;
    int m_numberOfThreads;
    int m_timeout;
};
#endif

