
#include "TruthParticle/TruthParticleConverter.h"
#include "TruthParticle/TruthParticle.h"
//#include "G4Kernel/macros.h"


using namespace xAOD;



bool TruthParticleConverter::convert( const TruthParticle *truth, TruthParticle_t &truth_t )
{
  if(truth){
    truth_t.pdgid = truth->pdgid();
    truth_t.e  = truth->e();
    truth_t.et = truth->et();
    truth_t.eta = truth->eta();
    truth_t.phi = truth->phi();
    truth_t.px = truth->px();
    truth_t.py = truth->py();
    truth_t.pz = truth->pz();
    return true;
  }
  return false;
  
}


bool TruthParticleConverter::convert( const TruthParticle_t &truth_t , TruthParticle *&truth )
{
  truth = new xAOD::TruthParticle( truth_t.e,
                                   truth_t.et,
                                   truth_t.eta,
                                   truth_t.phi,
                                   truth_t.px,
                                   truth_t.py,
                                   truth_t.pz,
                                   truth_t.pdgid
                                  );
  return true;
}




