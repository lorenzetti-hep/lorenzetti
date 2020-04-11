

#include "G4Kernel/PrimaryGeneratorAction.h"
#include "G4Event.hh"


PrimaryGeneratorAction::PrimaryGeneratorAction( PrimaryGenerator *gen): 
  G4VUserPrimaryGeneratorAction()
{
  // Need to copy the generator to works on thread mode
  m_generator = gen->copy();
  m_generator->initialize();
}


PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  m_generator->finalize();
  delete m_generator;
}


void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  if(m_generator)
    m_generator->GeneratePrimaryVertex(anEvent);
  else
    G4Exception("PrimaryGeneratorAction::GeneratePrimaries",
                "InvalidSetup", FatalException,
                "Generator is not instanciated.");
}



