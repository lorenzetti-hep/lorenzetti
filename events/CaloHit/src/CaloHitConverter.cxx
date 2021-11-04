#include "CaloHit/CaloHitConverter.h"
//#include "G4Kernel/macros.h"


using namespace xAOD;



bool CaloHitConverter::convert( const CaloHit *hit, CaloHit_t &hit_t )
{

  if(hit){
    hit_t.sampling    = (int)hit->sampling();
    hit_t.detector    = (int)hit->detector();
    hit_t.eta         = hit->eta();
    hit_t.phi         = hit->phi();
    hit_t.deta        = hit->deltaEta(); 
    hit_t.dphi        = hit->deltaPhi();
    hit_t.rmin        = hit->rMin();
    hit_t.rmax        = hit->rMax();
    hit_t.bcid_start  = hit->bcid_start();
    hit_t.bcid_end    = hit->bcid_end();
    hit_t.bc_duration = hit->bc_duration();
    hit_t.hash        = hit->hash();

    for ( int bcid = hit->bcid_start();  bcid <= hit->bcid_end(); ++bcid )
    {
      hit_t.edep.push_back( hit->edep(bcid) );
    }

    return true;
  }

  return false;
}



bool CaloHitConverter::convert( const CaloHit_t &hit_t, CaloHit *&hit )
{
  
  hit = new xAOD::CaloHit( hit_t.eta, 
                           hit_t.phi,
                           hit_t.deta,
                           hit_t.dphi,
                           hit_t.rmin,
                           hit_t.rmax,
                           hit_t.hash,
                           (CaloSampling)hit_t.sampling, 
                           (Detector)hit_t.detector,
                           hit_t.bc_duration,
                           hit_t.bcid_start,
                           hit_t.bcid_end );

  int pos=0;
  for ( int bcid = hit->bcid_start();  bcid <= hit->bcid_end(); ++bcid)
  {
    hit->edep( bcid, hit_t.edep.at(pos) ); // truth energy for each bunch crossing
    pos++;
  }


  
  return true;
}