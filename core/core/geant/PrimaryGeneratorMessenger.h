#ifndef PrimaryGeneratorMessenger_h
#define PrimaryGeneratorMessenger_h

/** geant 4 includes **/
#include "globals.hh"
#include "G4UImessenger.hh"


class PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;
class G4UIcmdWithAnInteger;
class G4UIcmdWithAString;

class PrimaryGeneratorMessenger : public G4UImessenger {

  public:
    PrimaryGeneratorMessenger(PrimaryGeneratorAction* genaction);
    ~PrimaryGeneratorMessenger();

    virtual void SetNewValue(G4UIcommand* command, std::string newValues);
    virtual std::string GetCurrentValue(G4UIcommand* command);

  private:
    PrimaryGeneratorAction* m_primaryAction;
    G4UIdirectory* m_dir;
    G4UIcmdWithAString* m_select;

};

#endif
