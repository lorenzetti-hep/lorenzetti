#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h


/** geant 4 includes **/
#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGunMessenger.hh"
#include "G4VPrimaryGenerator.hh"


class G4Event;



class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
  public:

    PrimaryGeneratorAction( G4VPrimaryGenerator *);
    ~PrimaryGeneratorAction();
  
    virtual void GeneratePrimaries(G4Event* anEvent);
  
  
  
  private:

    G4VPrimaryGenerator *m_generator;

};




#endif
