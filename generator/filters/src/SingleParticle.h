#ifndef filters_SingleParticle_h
#define filters_SingleParticle_h

#include "GenKernel/IAlgorithm.h"
#include "GenKernel/IGenerator.h"
#include "Pythia8/Pythia.h"


// NOTE: Only works with Pythia generator
namespace generator{

  class SingleParticle : public IAlgorithm
  {
    public:
      SingleParticle(const std::string , IGenerator *);
      ~SingleParticle()=default;
  
      virtual StatusCode initialize() override;
      virtual StatusCode execute( generator::Event &) override;
      virtual StatusCode finalize() override;
  
    protected:
    

      void fill(Pythia8::Pythia *gun, int id, float energy, float etaIn, float phiIn, bool atRest, bool hasLifetime);

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
      bool m_doRangedPhi;
      float m_phiMin;
      float m_phiMax;
     
  };
}
#endif
