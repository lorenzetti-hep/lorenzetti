
#include "core/reader/EventReaderMessenger.h"
#include "core/reader/EventReader.h"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"


EventReaderMessenger::EventReaderMessenger(EventReader* agen)
  : m_gen(agen)
{

  m_dir= new G4UIdirectory("/generator/eventReader/");
  m_dir->SetGuidance("Reading Jet event from an Werner's ROOT file");

  m_verbose= new G4UIcmdWithAnInteger("/generator/eventReader/verbose", this);
  m_verbose-> SetGuidance("Set verbose level");
  m_verbose-> SetParameterName("verboseLevel", false, false);
  m_verbose-> SetRange("verboseLevel>=0 && verboseLevel<=1");

  m_open= new G4UIcmdWithAString("/generator/eventReader/open", this);
  m_open-> SetGuidance("(re)open data file (Jet ROOT format)");
  m_open-> SetParameterName("input root file", true, true);
}


EventReaderMessenger::~EventReaderMessenger()
{
  delete m_verbose;
  delete m_open;
  delete m_dir;
}


void EventReaderMessenger::SetNewValue(G4UIcommand* command, G4String newValues)
{
  if (command==m_verbose) {
    int level= m_verbose-> GetNewIntValue(newValues);
    m_gen-> SetVerboseLevel(level);
  } else if (command==m_open) {
    m_gen-> SetFileName(newValues);
    G4cout << "EventReader root inputfile: " << m_gen->GetFileName() << G4endl;
    m_gen->Initialize();
  }
}


G4String EventReaderMessenger::GetCurrentValue(G4UIcommand* command)
{
  G4String cv;
  if (command == m_verbose) {
    cv= m_verbose->ConvertToString(m_gen-> GetVerboseLevel());
  } else  if (command == m_open) {
    cv= m_gen->GetFileName();
  }
  return cv;
}
