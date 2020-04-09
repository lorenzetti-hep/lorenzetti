#ifndef ActionInitialization_h
#define ActionInitialization_h

#include "GaugiKernel/Algorithm.h"
#include "DetectorConstruction.h"
#include "G4VUserActionInitialization.hh"

class G4VPrimaryGenerator;

class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization( G4VPrimaryGenerator *gen, std::vector<Gaugi::Algorithm*> acc, std::string output );
    virtual ~ActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

  private:

    std::string m_output;
    std::vector<Gaugi::Algorithm*> m_acc;
    SG::StoreGate *m_store;

    G4VPrimaryGenerator *m_generator;
};

#endif

