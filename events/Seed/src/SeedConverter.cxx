
#include "Seed/SeedConverter.h"
#include "Seed/Seed.h"
//#include "G4Kernel/macros.h"


using namespace xAOD;



bool SeedConverter::convert( const Seed *seed, Seed_t &seed_t )
{
  if(seed){
    seed_t.id = seed->id();
    seed_t.e  = seed->e();
    seed_t.et = seed->et();
    seed_t.eta = seed->eta();
    seed_t.phi = seed->phi();
    return true;
  }
  return false;
  
}


bool SeedConverter::convert( const Seed_t &seed_t , Seed *&seed )
{
  seed = new xAOD::Seed( seed_t.id
                         seed_t.e,
                         seed_t.et,
                         seed_t.eta,
                         seed_t.phi
                        );
  return true;
}




