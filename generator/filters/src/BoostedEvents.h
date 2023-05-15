#ifndef filters_BoostedEvents_h
#define filters_BoostedEvents_h

#include "GenKernel/IAlgorithm.h"
#include "GenKernel/IGenerator.h"
#include "SingleParticle.h"
#include "Pythia8/Pythia.h"

// NOTE: Only works with Pythia generator
namespace generator{

  class BoostedEvents : public SingleParticle
  {
    public:
      BoostedEvents(const std::string , IGenerator *);
      ~BoostedEvents();
  
      //virtual StatusCode execute( generator::Event &) override;
      virtual StatusCode initialize() override;
      virtual StatusCode execute( generator::Event &) override;
      virtual StatusCode finalize() override;
    
    private:
  
      float dR( float eta1, float phi1, float eta2, float phi2 ) const;
      void fill(Pythia8::Pythia *gun, int id, float energy, float etaIn, float phiIn, bool atRest, bool hasLifetime);

      float m_deltaR;
      bool m_hasLifetime;
      bool m_atRest;
      int m_pdgid;
     
  };
}
#endif
