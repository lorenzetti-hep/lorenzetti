
#include "G4Kernel/ActionInitialization.h"
#include "G4Kernel/PrimaryGeneratorAction.h"
#include "G4Kernel/RunAction.h"
#include "G4Kernel/EventAction.h"
#include "G4Kernel/SteppingAction.h"
#include "G4MTRunManager.hh"
#include <iostream>

ActionInitialization::ActionInitialization( int numberOfThreads,
                                            int timeout,
                                            PrimaryGenerator *gen,
                                            std::vector<Gaugi::Algorithm*> acc , 
                                            std::string output)
 : 
  IMsgService("ActionInitialization"), 
  G4VUserActionInitialization(),
  m_acc(acc),
  m_generator(gen),
  m_output(output),
  m_numberOfThreads(numberOfThreads),
  m_timeout(timeout)
{

  for ( auto toolHandle : m_acc )
  { 
    MSG_INFO( "Initializing the tool with name " << toolHandle->name() );
    if ( toolHandle->initialize().isFailure() )
    {
      MSG_ERROR("It's not possible to initialize the tool with name: " << toolHandle->name() );
    }
  } 

}


ActionInitialization::~ActionInitialization()
{
  MSG_INFO( "~ActionInitialization()" );
  for ( auto toolHandle : m_acc )
  { 
    if ( toolHandle->finalize().isFailure() )
    {
      MSG_ERROR("It's not possible to finalize the tool with name: " << toolHandle->name() );
    }
  }
}


void ActionInitialization::BuildForMaster() const
{
  //SetUserAction(new RunAction(m_acc, m_output));
}


void ActionInitialization::Build() const
{
  MSG_INFO( "Build()" );
  SetUserAction(new PrimaryGeneratorAction(m_generator));
  SetUserAction(new RunAction(m_numberOfThreads, m_timeout, m_acc, m_output));
  SetUserAction(new EventAction());
  SetUserAction(new SteppingAction());
}  

