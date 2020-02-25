
#include "core/geant/ActionInitialization.h"
#include "core/geant/PrimaryGeneratorAction.h"
#include "core/geant/RunAction.h"
#include "core/geant/EventAction.h"
#include "core/geant/SteppingAction.h"
#include "core/geant/DetectorConstruction.h"
#include "G4MTRunManager.hh"


ActionInitialization::ActionInitialization(std::vector<IAlgTool*> sequence , std::string output)
 : G4VUserActionInitialization(),
  m_sequence(sequence),
  m_output(output)
{;}


ActionInitialization::~ActionInitialization()
{;}


void ActionInitialization::BuildForMaster() const
{
  SetUserAction(new RunAction(m_sequence, m_output));
}


void ActionInitialization::Build() const
{
  SetUserAction(new PrimaryGeneratorAction());
  SetUserAction(new RunAction(m_sequence, m_output));
  SetUserAction(new EventAction());
  SetUserAction(new SteppingAction());
}  

