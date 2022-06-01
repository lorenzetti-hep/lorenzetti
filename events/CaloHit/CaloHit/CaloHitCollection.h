#ifndef CaloHitCollection_h
#define CaloHitCollection_h

/** simulator includes **/
#include "CaloHit/CaloHit.h"
#include "GaugiKernel/Collection.h"

namespace xAOD{
  // All objects inside of the data vector will be const
  typedef SG::Collection<xAOD::CaloHit> CaloHitCollection;
}
#endif
