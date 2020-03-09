#ifndef TruthParticleContainer_h
#define TruthParticleContainer_h

/** simulator includes **/
#include "TruthParticle/TruthParticle.h"
#include "core/DataVector.h"

namespace xAOD{
  typedef DataVector<const xAOD::TruthParticle> TruthParticleContainer;
}
#endif
