#ifndef PythiaGenerator_BoostedEvents_h
#define PythiaGenerator_BoostedEvents_h

#include "Pythia8/Pythia.h"
#include "P8Kernel/Algorithm.h"
#include "ParticleGun.h"

namespace generator{

  class BoostedEvents : public ParticleGun
  {
    public:
      BoostedEvents();
      ~BoostedEvents();
  
      virtual StatusCode initialize() override;
      virtual StatusCode execute( generator::Event &) override;
      virtual StatusCode finalize() override;
  
    private:
  
      float dR( float eta1, float phi1, float eta2, float phi2 ) const;

      float m_deltaR;
      bool m_hasLifetime;
      bool m_atRest;
      int m_pdgid;
     
  };
}
#endif
