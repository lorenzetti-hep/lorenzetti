
#include "helper.h"
#include <math.h>

namespace generator{

  namespace ParticleHelper{


      // particles with strong or electric charge, or composed of ones having it, which thereby 
      // should be considered visible in a normal detector.
      bool isVisible(HepMC3::GenParticle *particle)
      {
        // Take from: https://github.com/lorenzetti-hep/pythia8/blob/main/src/ParticleData.cc
        // A particle is invisible if it has neither strong nor electric charge,
        // and is not made up of constituents that have it. Only relevant for
        // long-lived particles. This list may need to be extended.
        const int INVISIBLENUMBER = 49;
        const int INVISIBLETABLE[80] = {
               12,      14,      16,      18,      23,      25,      32,      33,
               35,      36,      39,      41,      45,      46,      51,      52,
               53,      54,      55,      56,      57,      58,      59,      60,
          1000012, 1000014, 1000016, 1000018, 1000022, 1000023, 1000025, 1000035,
          1000045, 1000039, 2000012, 2000014, 2000016, 2000018, 4900012, 4900014,
          4900016, 4900021, 4900022, 5000039, 5100039, 9900012, 9900014, 9900016,
          9900023,       0,       0,       0,       0,       0,       0,       0,
                0,       0,       0,       0,       0,       0,       0,       0,
                0,       0,       0,       0,       0,       0,       0,       0,
                0,       0,       0,       0,       0,       0,       0,       0
        };
        // A particle is invisible if in current table of such.
        bool isVisibleSave = true;
        int idSave = particle->pid();
        for (int i = 0; i < INVISIBLENUMBER; ++i) 
            if (idSave == INVISIBLETABLE[i]) isVisibleSave = false;

        // Additionally all particles purely in Hidden Sector are invisible.
        if (idSave > 4900100 && idSave < 4909000) isVisibleSave = false;
        return isVisibleSave;
      }


      //
      // From: https://pythia.org/latest-manual/ParticleProperties.html
      // true for a remaining particle, i.e. one with positive status code, else false. 
      // Thus, after an event has been fully generated, it separates the final-state particles 
      // from intermediate-stage ones. (If used earlier in the generation process, a particle then 
      // considered final may well decay later.)
      //
      // HepMC has a different code table: https://github.com/lorenzetti-hep/pythia8/blob/main/src/Event.cc#L381
      bool isFinal(const HepMC3::GenParticle *particle){
        return particle->status() == 1 ? true : false;
      }

      float et(const HepMC3::GenParticle *particle){
        float eta = particle->momentum().eta();
        float et = eta != 0.0 ? (particle->momentum().e())/cosh(fabs(eta)) : 0.0;
        return et;
      }

      bool isCharged(const HepMC3::GenParticle *particle){
        return particle->pid() > 0? false : true;
      }


      void ParticleFilter::filter(HepMC3::GenEvent &event) {
      
        // Reset arrays in preparation for new event.
        //m_keptIndx.resize(0);
        m_keptPtrs.resize(0);
        // Loop over all particles in the event record.
        for (const auto p : event.particles()){
          // Skip if particle kind selection criteria not fulfilled.
          if (!ParticleHelper::isFinal(p.get())) continue;
          if (m_select == 2 && !ParticleHelper::isVisible(p.get())) continue;
          bool isCharged = ParticleHelper::isCharged(p.get());
          if (m_select == 3 && !isCharged) continue;
          // Skip if too large pseudorapidity.
          float eta = p->momentum().eta();
          if (abs(eta) > m_etaMax) continue;
          if (abs(eta) < m_etaMin) continue;
          // Skip if too small pT.
          float pT = p->momentum().pt();
          if       (isCharged && pT < m_pTminCharged) continue;
          else if (!isCharged && pT < m_pTminNeutral) continue;
          // Add particle to vectors of indices and pointers.
          //m_keptIndx.push_back( i );
          m_keptPtrs.push_back( p.get() );
        // End of particle loop. Done.
        }
      }



  }



  


}


