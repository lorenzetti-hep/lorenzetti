#ifndef ActionInitialization_h
#define ActionInitialization_h

#include "GaugiKernel/Algorithm.h"

/** simulator libs **/
#include "DetectorConstruction.h"

/** geant 4 libs **/
#include "G4VUserActionInitialization.hh"


class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization( std::vector<Gaugi::Algorithm*> acc, std::string output );
    virtual ~ActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

  private:

    std::vector<Gaugi::Algorithm*> m_acc;
    std::string   m_output;
};

#endif

