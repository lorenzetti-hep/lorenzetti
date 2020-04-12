#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h


/** geant 4 includes **/
#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGunMessenger.hh"
#include "G4VPrimaryGenerator.hh"
#include "G4Kernel/PrimaryGenerator.h"
#include "GaugiKernel/MsgStream.h"

class G4Event;



class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction, public MsgService {
  public:

    PrimaryGeneratorAction( PrimaryGenerator *);
    ~PrimaryGeneratorAction();
  
    virtual void GeneratePrimaries(G4Event* anEvent);
  
  
  
  private:

    PrimaryGenerator *m_generator;

};




#endif
