#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h


/** geant 4 libs **/
#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGunMessenger.hh"

/** standard libs **/
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
    void SetGenerator(std::string genname);
  
    G4VPrimaryGenerator* GetGenerator() const;
    std::string GetGeneratorName() const;
  
  private:

  
    G4VPrimaryGenerator       *m_currentGenerator;
    std::string                m_currentGeneratorName;
    PrimaryGeneratorMessenger *m_messenger;

    std::map<std::string, G4VPrimaryGenerator*>   m_generatorMap;

};




#endif
