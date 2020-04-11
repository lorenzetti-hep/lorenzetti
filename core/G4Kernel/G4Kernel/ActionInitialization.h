#ifndef ActionInitialization_h
#define ActionInitialization_h

#include "GaugiKernel/Algorithm.h"
#include "G4Kernel/PrimaryGenerator.h"
#include "G4VUserActionInitialization.hh"


class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization( PrimaryGenerator *gen, std::vector<Gaugi::Algorithm*> acc, std::string output );
    virtual ~ActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

  private:

    std::string m_output;
    std::vector<Gaugi::Algorithm*> m_acc;
    SG::StoreGate *m_store;
    PrimaryGenerator *m_generator;
};

#endif

