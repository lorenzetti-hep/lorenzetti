#ifndef EventSeedContainer_h
#define EventSeedContainer_h

/** simulator includes **/
#include "EventInfo/EventSeed.h"
#include "GaugiKernel/DataVector.h"

namespace xAOD{
  typedef SG::DataVector<xAOD::EventSeed> EventSeedContainer;
}
#endif
