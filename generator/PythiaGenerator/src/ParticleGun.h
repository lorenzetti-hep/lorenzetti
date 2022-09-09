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
  
    protected:
  
      void fillParticle( int id, double energy, double etaIn, double phiIn, bool atRest, bool hasLifetime); 
  

      void clearParticles();
  
      float m_eta;
      float m_phi;
      float m_energy;
      float m_energyMin;
      float m_energyMax;
      bool m_hasLifetime;
      bool m_atRest;
      int m_pdgid;
      bool m_doRangedEta;
      float m_etaMin;
      float m_etaMax;
     
  };
}
#endif
