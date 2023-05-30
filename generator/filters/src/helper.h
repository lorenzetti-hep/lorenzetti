#ifndef GenKernel_helper_h
#define GenKernel_helper_h

#include "HepMC3/GenVertex.h"
#include "HepMC3/GenParticle.h"   
#include "HepMC3/Units.h"



namespace generator{

  //
  // Helpers
  //
  namespace ParticleHelper{

    //
    // particles with strong or electric charge, or composed of ones having it, which thereby 
    // should be considered visible in a normal detector.
    //
    bool isVisible(const HepMC3::GenParticle* );

    //
    // From: https://pythia.org/latest-manual/ParticleProperties.html
    // true for a remaining particle, i.e. one with positive status code, else false. 
    // Thus, after an event has been fully generated, it separates the final-state particles 
    // from intermediate-stage ones. (If used earlier in the generation process, a particle then 
    // considered final may well decay later.)
    //
    // HepMC has a different code table: https://github.com/lorenzetti-hep/pythia8/blob/main/src/Event.cc#L381
    //
    bool isFinal( const HepMC3::GenParticle* );

    bool isCharged(const HepMC3::GenParticle* );

    float et( const HepMC3::GenParticle* );

  
    class ParticleFilter {
    
      public:
        // Constructor sets properties of filter.
        ParticleFilter( int selectIn, float etaMaxIn = 50., float etaMinIn = 0.,
                        float pTminChargedIn = 0., float pTminNeutralIn = 0.)
                        : m_select(selectIn), 
                          m_etaMax(etaMaxIn), 
                          m_etaMin(etaMinIn),
                          m_pTminCharged(pTminChargedIn),
                          m_pTminNeutral(pTminNeutralIn) {}

        ~ParticleFilter()=default;

        // Analysis of each new event to find acceptable particles.
        void filter(HepMC3::GenEvent &event);

        // Return size of array, and index of a particle.
        int size()       const {return m_keptPtrs.size();}

        // Return pointer or reference to a particle.
        const HepMC3::GenParticle* particlePtr(int i) {return  m_keptPtrs[i];}
        const HepMC3::GenParticle& particleRef(int i) {return *m_keptPtrs[i];}

        std::vector<const HepMC3::GenParticle*>& getParticlesRef() { return m_keptPtrs; }

      private:

        // Filter properties, set by constructor.
        int    m_select;
        float m_etaMax;
        float m_etaMin;
        float m_pTminCharged;
        float m_pTminNeutral;
        // Kept particle pointers, referring to original event.
        std::vector<const HepMC3::GenParticle*> m_keptPtrs;

    };

  }
}


#endif




