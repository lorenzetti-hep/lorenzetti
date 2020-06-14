#ifndef PythiaGenerator_ParticleGun_h
#define PythiaGenerator_ParticleGun_h

#include "Pythia8/Pythia.h"
#include "P8Kernel/Algorithm.h"


namespace generator{

  class ParticleGun : public Algorithm
  {
    public:
      ParticleGun();
      ~ParticleGun();
  
      virtual StatusCode initialize() override;
      virtual StatusCode execute( generator::Event &) override;
      virtual StatusCode finalize() override;
  
    private:
  
      void fillParticle( Pythia8::Event& event,int id, double emin, double emax, double thetaIn, double phiIn,
                         Pythia8::ParticleData& pdt, bool atRest, bool hasLifetime); 
  
  
  
      float m_theta;
      float m_eta;
      float m_phi;
      float m_energy;
      float m_energyMin;
      float m_energyMax;
      bool m_hasLifetime;
      bool m_atRest;
      int m_pdgid;
      int m_numberOfParticles;
     
  };
}
#endif
