#ifndef filters_SingleParticle_h
#define filters_SingleParticle_h

#include "GenKernel/IAlgorithm.h"
#include "GenKernel/IGenerator.h"

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
