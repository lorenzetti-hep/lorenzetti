

#include "core/geant/PrimaryGeneratorAction.h"
#include "core/geant/PrimaryGeneratorMessenger.h"
#include "core/reader/EventReader.h"


#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleGunMessenger.hh"



PrimaryGeneratorAction::PrimaryGeneratorAction(): G4VUserPrimaryGeneratorAction()
{
  m_generatorMap["particleGun"] = new G4ParticleGun();
  m_generatorMap["eventReader"]   = new EventReader();
  
  m_currentGenerator = m_generatorMap["particleGun"];
  m_currentGeneratorName= "particleGun";
  m_messenger= new PrimaryGeneratorMessenger(this);
}


PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete m_messenger;
}


void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  if(m_currentGenerator)
    m_currentGenerator->GeneratePrimaryVertex(anEvent);
  else
    G4Exception("PrimaryGeneratorAction::GeneratePrimaries",
                "InvalidSetup", FatalException,
                "Generator is not instanciated.");
}



void PrimaryGeneratorAction::SetGenerator(G4VPrimaryGenerator* gen)
{
  m_currentGenerator= gen;
}

void PrimaryGeneratorAction::SetGenerator(G4String genname)
{
  std::map<G4String, G4VPrimaryGenerator*>::iterator pos = m_generatorMap.find(genname);

  if(pos != m_generatorMap.end()) {
    m_currentGenerator= pos->second;
    m_currentGeneratorName= genname;
  }
}

G4VPrimaryGenerator* PrimaryGeneratorAction::GetGenerator() const
{
  return m_currentGenerator;
}

G4String PrimaryGeneratorAction::GetGeneratorName() const
{
  return m_currentGeneratorName;
}

