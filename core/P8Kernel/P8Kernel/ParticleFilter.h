#ifndef P8Kernel_ParticleFilter_h
#define P8Kernel_ParticleFilter_h

#include <iostream>
#include "Pythia8/Pythia.h"
#include "Pythia8/HeavyIons.h"
#include <vector>

namespace generator{

  class ParticleFilter {
  
    public:
      // Constructor sets properties of filter.
      ParticleFilter( int selectIn, double etaMaxIn = 50.,
                      double pTminChargedIn = 0., double pTminNeutralIn = 0.)
                      : m_select(selectIn), m_etaMax(etaMaxIn), m_pTminCharged(pTminChargedIn),
                        m_pTminNeutral(pTminNeutralIn) {}
    
      ~ParticleFilter()=default;
  
      // Analysis of each new event to find acceptable particles.
      void filter(Pythia8::Event& event);
    
      // Return size of array, and index of a particle.
      int size()       const {return m_keptPtrs.size();}
      int index(int i) const {return m_keptIndx[i];}
    
      // Return pointer or reference to a particle.
      Pythia8::Particle* particlePtr(int i) {return  m_keptPtrs[i];}
      Pythia8::Particle& particleRef(int i) {return *m_keptPtrs[i];}
    
      // List kept particles only.
      void list(std::ostream& os = std::cout);
    
      std::vector<Pythia8::Particle*>& getParticlesRef() { return m_keptPtrs; }
    
    private:
    
      // Filter properties, set by constructor.
      int    m_select;
      double m_etaMax;
      double m_pTminCharged;
      double m_pTminNeutral;
      // Kept particle indices and pointers, referring to original event.
      std::vector<int>       m_keptIndx;
      std::vector<Pythia8::Particle*> m_keptPtrs;
    
  };
}
#endif
