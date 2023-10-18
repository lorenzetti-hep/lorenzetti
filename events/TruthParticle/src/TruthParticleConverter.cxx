
#include "TruthParticle/TruthParticleContainer.h"
#include "TruthParticle/TruthParticleConverter.h"


using namespace xAOD;



bool TruthParticleConverter::convert( const TruthParticle *truth, TruthParticle_t &truth_t ) const
{
  if(truth){
    truth_t.pdgid  = truth->pdgid();
    truth_t.seedid = truth->seedid();
    truth_t.e      = truth->e();
    truth_t.et     = truth->et();
    truth_t.eta    = truth->eta();
    truth_t.phi    = truth->phi();
    truth_t.px     = truth->px();
    truth_t.py     = truth->py();
    truth_t.pz     = truth->pz();
    truth_t.vx     = truth->vx();
    truth_t.vy     = truth->vy();
    truth_t.vz     = truth->vz();

    return true;
  }
  return false;
}


bool TruthParticleConverter::convert( const TruthParticle_t &truth_t , TruthParticle *&truth ) const
{
  truth = new xAOD::TruthParticle( 
                                   truth_t.pdgid,
                                   truth_t.seedid,
                                   truth_t.e,
                                   truth_t.et,
                                   truth_t.eta,
                                   truth_t.phi,
                                   truth_t.px,
                                   truth_t.py,
                                   truth_t.pz,
                                   truth_t.vx,
                                   truth_t.vy,
                                   truth_t.vz);                                  
  return true;
}

