#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h


/** geant 4 includes **/
#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGunMessenger.hh"

/** standard includes **/
#include <map>
#include <string>

class G4Event;
class G4VPrimaryGenerator;
class PrimaryGeneratorMessenger;



class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
  public:

    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction();
  
    virtual void GeneratePrimaries(G4Event* anEvent);
  
    void SetGenerator(G4VPrimaryGenerator* gen);
    void SetGenerator(G4String genname);
  
    G4VPrimaryGenerator* GetGenerator() const;
    G4String GetGeneratorName() const;
  
  private:

  
    G4VPrimaryGenerator       *m_currentGenerator;
    G4String                m_currentGeneratorName;
    PrimaryGeneratorMessenger *m_messenger;

    std::map<G4String, G4VPrimaryGenerator*>   m_generatorMap;

};




#endif
