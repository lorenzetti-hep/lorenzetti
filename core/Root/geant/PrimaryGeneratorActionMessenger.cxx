

#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIparameter.hh"

#include "core/geant/PrimaryGeneratorMessenger.h"
#include "core/geant/PrimaryGeneratorAction.h"


PrimaryGeneratorMessenger::PrimaryGeneratorMessenger( PrimaryGeneratorAction* genaction )
  : G4UImessenger(),
    m_primaryAction(genaction)
{
  m_dir= new G4UIdirectory("/generator/");
  m_dir-> SetGuidance("Control commands for primary generator");


  m_select= new G4UIcmdWithAString("/generator/select", this);
  m_select-> SetGuidance("Select generator type");
  m_select-> SetParameterName("generator_type", false, false);
  m_select-> SetCandidates("particleGun eventReader");
  m_select-> SetDefaultValue("particleGun");
}


PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
  delete m_select;
  delete m_dir;
}


void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValues)
{
  if ( command == m_select) {
    m_primaryAction->SetGenerator(newValues);
    G4cout << "current generator type: " << m_primaryAction-> GetGeneratorName() << G4endl;
  }
}


G4String PrimaryGeneratorMessenger::GetCurrentValue(G4UIcommand* command)
{
  G4String cv, st;
  if (command == m_select) {
    cv= m_primaryAction-> GetGeneratorName();
  }

 return cv;
}
