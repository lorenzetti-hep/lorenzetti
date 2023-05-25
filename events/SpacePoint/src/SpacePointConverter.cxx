#include "SpacePoint/SpacePointConverter.h"
//#include "G4Kernel/macros.h"


using namespace xAOD;



bool SpacePointConverter::convert( const SpacePoint *sp, SpacePoint_t &sp_t )
{

  if(sp){
    sp_t.sampling    = (int)sp->sampling();
    sp_t.detector    = (int)sp->detector();
    sp_t.x           = sp->x();
    sp_t.y           = sp->y();
    sp_t.z           = sp->z();
    sp_t.phi         = sp->phi();
    sp_t.dz          = sp->deltaZ(); 
    sp_t.dphi        = sp->deltaPhi();
    sp_t.bcid_start  = sp->bcid_start();
    sp_t.bcid_end    = sp->bcid_end();
    sp_t.bc_duration = sp->bc_duration();
    sp_t.hash        = sp->hash();

    for ( int bcid = sp->bcid_start();  bcid <= sp->bcid_end(); ++bcid )
    {
      sp_t.edep.push_back( sp->edep(bcid) );
    }

    return true;
  }

  return false;
}



bool SpacePointConverter::convert( const SpacePoint_t &sp_t, SpacePoint *&sp )
{
  
  sp = new xAOD::SpacePoint( 
                           sp_t.x, 
                           sp_t.y, 
                           sp_t.z, 
                           sp_t.phi,
                           sp_t.dz,
                           sp_t.dphi,
                           sp_t.hash,
                           (InnerSampling)sp_t.sampling, 
                           (InnerDetector)sp_t.detector,
                           sp_t.bc_duration,
                           sp_t.bcid_start,
                           sp_t.bcid_end );

  int pos=0;
  for ( int bcid = sp->bcid_start();  bcid <= sp->bcid_end(); ++bcid)
  {
    sp->edep( bcid, sp_t.edep.at(pos) ); // truth energy for each bunch crossing
    pos++;
  }


  
  return true;
}