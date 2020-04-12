

#include "G4Kernel/PrimaryGeneratorAction.h"
#include "G4Event.hh"


PrimaryGeneratorAction::PrimaryGeneratorAction( PrimaryGenerator *gen): 
  IMsgService("PrimaryGeneratorAction"),
  G4VUserPrimaryGeneratorAction()
{
  // Need to copy the generator to works on thread mode
  m_generator = gen->copy();
  MSG_INFO( "Create the generator copy" );
  m_generator->initialize();
}


PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  m_generator->finalize();
  delete m_generator;
}


void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  if(m_generator){
    MSG_INFO( "GeneratePrimaryVertex..." );
    m_generator->GeneratePrimaryVertex(anEvent);
  }else
    G4Exception("PrimaryGeneratorAction::GeneratePrimaries",
                "InvalidSetup", FatalException,
                "Generator is not instanciated.");
}



