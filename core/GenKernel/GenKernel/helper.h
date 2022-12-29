#ifndef GenKernel_helper_h
#define GenKernel_helper_h

#include "HepMC3/GenVertex.h"
#include "HepMC3/GenParticle.h"   
#include "HepMC3/Units.h"



namespace generator{

  namespace ParticleHelper{

    bool isVisible(const HepMC3::GenParticle* );
    bool isFinal(  const HepMC3::GenParticle* );
    bool isCharged(const HepMC3::GenParticle* );
    float et(      const HepMC3::GenParticle* );

  }


  class ParticleFilter {
  
    public:
      // Constructor sets properties of filter.
      ParticleFilter( int selectIn, double etaMaxIn = 50.,
                      double pTminChargedIn = 0., double pTminNeutralIn = 0.)
                      : m_select(selectIn), 
                        m_etaMax(etaMaxIn), 
                        m_pTminCharged(pTminChargedIn),
                        m_pTminNeutral(pTminNeutralIn) {}
    
      ~ParticleFilter()=default;
  
      // Analysis of each new event to find acceptable particles.
      void filter(HepMC3::GenEvent &event);
    
      // Return size of array, and index of a particle.
      int size()       const {return m_keptPtrs.size();}
      //int index(int i) const {return m_keptIndx[i];}
    
      // Return pointer or reference to a particle.
      const HepMC3::GenParticle* particlePtr(int i) {return  m_keptPtrs[i];}
      const HepMC3::GenParticle& particleRef(int i) {return *m_keptPtrs[i];}
    
      // List kept particles only.
      //void list(std::ostream& os = std::cout);
    
      std::vector<const HepMC3::GenParticle*>& getParticlesRef() { return m_keptPtrs; }
    
    private:
    
      // Filter properties, set by constructor.
      int    m_select;
      double m_etaMax;
      double m_pTminCharged;
      double m_pTminNeutral;
      // Kept particle indices and pointers, referring to original event.
      //std::vector<int>       m_keptIndx;
      std::vector<const HepMC3::GenParticle*> m_keptPtrs;
    
  };

}


#endif




