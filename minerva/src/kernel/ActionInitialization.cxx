
#include "kernel/ActionInitialization.h"
#include "kernel/PrimaryGeneratorAction.h"
#include "kernel/RunAction.h"
#include "kernel/EventAction.h"
#include "kernel/SteppingAction.h"
#include "geometry/DetectorConstruction.h"
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

