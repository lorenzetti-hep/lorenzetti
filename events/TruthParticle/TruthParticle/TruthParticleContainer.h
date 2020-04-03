#ifndef TruthParticleContainer_h
#define TruthParticleContainer_h

/** simulator includes **/
#include "TruthParticle/TruthParticle.h"
#include "GaugiKernel/DataVector.h"

namespace xAOD{
  typedef SG::DataVector<xAOD::TruthParticle> TruthParticleContainer;
}
#endif
