
#include "EventInfo/EventSeedConverter.h"


using namespace xAOD;


bool EventSeedConverter::convert( const EventSeed *seed, EventSeed_t &seed_t ) const
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


bool EventSeedConverter::convert( const EventSeed_t &seed_t , EventSeed *&seed ) const
{
  seed = new xAOD::EventSeed( seed_t.id,
                              seed_t.e,
                              seed_t.et,
                              seed_t.eta,
                              seed_t.phi
                             );
  return true;
}
