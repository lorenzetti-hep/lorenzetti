
#include "TruthParticle/ParticleSeedConverter.h"
#include "TruthParticle/ParticleSeed.h"
//#include "G4Kernel/macros.h"


using namespace xAOD;



bool ParticleSeedConverter::convert( const ParticleSeed *truth, ParticleSeed_t &truth_t )
{
  if(truth){
    // truth_t.pdgid = truth->pdgid();
    truth_t.etot  = truth->etot();
    truth_t.ettot = truth->ettot();
    truth_t.eta = truth->eta();
    truth_t.phi = truth->phi();
    // truth_t.px = truth->px();
    // truth_t.py = truth->py();
    // truth_t.pz = truth->pz();
    // truth_t.vx = truth->vx();
    // truth_t.vy = truth->vy();
    // truth_t.vz = truth->vz();
    // truth_t.seedEta = truth->seedEta();
    // truth_t.seedPhi = truth->seedPhi();
    // truth_t.seedEtot = truth->seedEtot();
    // truth_t.seedEttot = truth->seedEttot();

    return true;
  }
  return false;
  
}


bool ParticleSeedConverter::convert( const ParticleSeed_t &truth_t , ParticleSeed *&truth )
{
  truth = new xAOD::ParticleSeed(   truth_t.etot,
                                    truth_t.ettot,
                                    truth_t.eta,
                                    truth_t.phi);
                                    // truth_t.pdgid);
                                //    truth_t.px,
                                //    truth_t.py,
                                //    truth_t.pz,
                                //    truth_t.pdgid,
                                //    truth_t.vx,
                                //    truth_t.vy,
                                //    truth_t.vz);
                                  //  truth_t.seedEta,
                                  //  truth_t.seedPhi,
                                  //  truth_t.seedEtot,
                                  //  truth_t.seedEttot
                                  
  return true;
}




