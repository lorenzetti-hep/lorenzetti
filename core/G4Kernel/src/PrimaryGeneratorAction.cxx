

#include "G4Kernel/PrimaryGeneratorAction.h"
#include "G4Kernel/PrimaryGeneratorMessenger.h"
#include "G4Kernel/EventReader.h"
#include "G4Kernel/ParticleGun.h"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleGunMessenger.hh"
#include "G4GeneralParticleSource.hh"


PrimaryGeneratorAction::PrimaryGeneratorAction( G4VPrimaryGenerator *gen): G4VUserPrimaryGeneratorAction()
{
  m_generator = gen;
}


PrimaryGeneratorAction::~PrimaryGeneratorAction()
{;}


void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  if(m_generator)
    m_currentGenerator->GeneratePrimaryVertex(anEvent);
  else
    G4Exception("PrimaryGeneratorAction::GeneratePrimaries",
                "InvalidSetup", FatalException,
                "Generator is not instanciated.");
}



