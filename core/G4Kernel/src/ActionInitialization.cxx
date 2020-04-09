
#include "G4Kernel/ActionInitialization.h"
#include "G4Kernel/PrimaryGeneratorAction.h"
#include "G4Kernel/RunAction.h"
#include "G4Kernel/EventAction.h"
#include "G4Kernel/SteppingAction.h"
#include "G4Kernel/DetectorConstruction.h"
#include "G4MTRunManager.hh"
#include <iostream>

ActionInitialization::ActionInitialization( G4VPrimaryGenerator *gen,
                                            std::vector<Gaugi::Algorithm*> acc , 
                                            std::string output)
 : G4VUserActionInitialization(),
  m_acc(acc),
  m_output(output),
  m_generator(gen)
{
  m_store = new SG::StoreGate(output);  

  for ( auto toolHandle : m_acc )
  { 
    toolHandle->setStoreGateSvc( m_store );
    if ( toolHandle->initialize().isFailure() )
    {
      // raise exception here
    }
  } 

}


ActionInitialization::~ActionInitialization()
{
  for ( auto toolHandle : m_acc )
  { 
    if ( toolHandle->finalize().isFailure() )
    {
      // raise exception here
    }
  }
  delete m_store;
}


void ActionInitialization::BuildForMaster() const
{
  SetUserAction(new RunAction(m_acc));
}


void ActionInitialization::Build() const
{
  SetUserAction(new PrimaryGeneratorAction(m_generator));
  SetUserAction(new RunAction(m_acc));
  SetUserAction(new EventAction());
  SetUserAction(new SteppingAction());
}  

