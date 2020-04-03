
#include "G4Kernel/ActionInitialization.h"
#include "G4Kernel/PrimaryGeneratorAction.h"
#include "G4Kernel/RunAction.h"
#include "G4Kernel/EventAction.h"
#include "G4Kernel/SteppingAction.h"
#include "G4Kernel/DetectorConstruction.h"
#include "G4MTRunManager.hh"


ActionInitialization::ActionInitialization(std::vector<Gaugi::Algorithm*> acc , 
                                           std::string output)
 : G4VUserActionInitialization(),
  m_acc(acc),
  m_output(output)
{;}


ActionInitialization::~ActionInitialization()
{;}


void ActionInitialization::BuildForMaster() const
{
  SetUserAction(new RunAction(m_acc, m_output));
}


void ActionInitialization::Build() const
{
  SetUserAction(new PrimaryGeneratorAction());
  SetUserAction(new RunAction(m_acc, m_output));
  SetUserAction(new EventAction());
  SetUserAction(new SteppingAction());
}  

