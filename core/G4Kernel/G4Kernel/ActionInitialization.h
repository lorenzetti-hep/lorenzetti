#ifndef ActionInitialization_h
#define ActionInitialization_h

#include "GaugiKernel/MsgStream.h"
#include "GaugiKernel/Algorithm.h"
#include "G4Kernel/PrimaryGenerator.h"
#include "G4VUserActionInitialization.hh"


class ActionInitialization : public G4VUserActionInitialization, public MsgService
{
  public:
    ActionInitialization( int numberOfThreads, int timeout, PrimaryGenerator *gen, std::vector<Gaugi::Algorithm*> acc, std::string output );
    virtual ~ActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

  private:


    std::vector<Gaugi::Algorithm*> m_acc;
    PrimaryGenerator *m_generator;
    std::string m_output;
    int m_numberOfThreads;
    int m_timeout;
};

#endif

