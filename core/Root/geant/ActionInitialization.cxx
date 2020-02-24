
#include "core/geant/ActionInitialization.h"
#include "core/geant/PrimaryGeneratorAction.h"
#include "core/geant/RunAction.h"
#include "core/geant/EventAction.h"
#include "core/geant/SteppingAction.h"
#include "core/geant/DetectorConstruction.h"
#include "G4MTRunManager.hh"


ActionInitialization::ActionInitialization(DetectorConstruction * /**/, std::string output)
 : G4VUserActionInitialization(),
  m_output(output)
{;}


ActionInitialization::~ActionInitialization()
{;}


void ActionInitialization::BuildForMaster() const
{
  SetUserAction(new RunAction());
}


void ActionInitialization::Build() const
{
  SetUserAction(new PrimaryGeneratorAction());
  SetUserAction(new RunAction());
  SetUserAction(new EventAction());
  SetUserAction(new SteppingAction());
}  

