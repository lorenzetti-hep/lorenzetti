#ifndef SpacePointCollection_h
#define SpacePointCollection_h

/** simulator includes **/
#include "SpacePoint/SpacePoint.h"
#include "GaugiKernel/Collection.h"

namespace xAOD{
  // All objects inside of the data vector will be const
  typedef SG::Collection<xAOD::SpacePoint> SpacePointCollection;
}
#endif
