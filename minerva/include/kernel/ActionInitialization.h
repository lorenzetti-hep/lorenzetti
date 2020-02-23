#ifndef ActionInitialization_h
#define ActionInitialization_h

/** simulator libs **/
#include "geometry/DetectorConstruction.h"

/** geant 4 libs **/
#include "G4VUserActionInitialization.hh"


class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization(DetectorConstruction *, std::string output );
    virtual ~ActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

  private:

    std::string   m_output;
};

#endif

