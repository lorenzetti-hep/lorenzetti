#ifndef SpacePointContainer_h
#define SpacePointContainer_h

/** simulator includes **/
#include "SpacePoint/SpacePoint.h"
#include "GaugiKernel/DataVector.h"

namespace xAOD{
  // All objects inside of the data vector will be const
  typedef SG::DataVector<xAOD::SpacePoint> SpacePointContainer;
}
#endif
