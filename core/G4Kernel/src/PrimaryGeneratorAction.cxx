

#include "G4Kernel/PrimaryGeneratorAction.h"
#include "G4Event.hh"


PrimaryGeneratorAction::PrimaryGeneratorAction( G4VPrimaryGenerator *gen): G4VUserPrimaryGeneratorAction()
{
  m_generator = gen;
}


PrimaryGeneratorAction::~PrimaryGeneratorAction()
{;}


void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  if(m_generator)
    m_generator->GeneratePrimaryVertex(anEvent);
  else
    G4Exception("PrimaryGeneratorAction::GeneratePrimaries",
                "InvalidSetup", FatalException,
                "Generator is not instanciated.");
}



