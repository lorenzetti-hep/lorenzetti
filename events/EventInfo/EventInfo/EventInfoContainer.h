#ifndef EventInfoContainer_h
#define EventInfoContainer_h

/** simulator includes **/
#include "EventInfo/EventInfo.h"
#include "core/DataVector.h"

namespace xAOD{
  typedef DataVector<const xAOD::EventInfo> EventInfoContainer;
}
#endif
