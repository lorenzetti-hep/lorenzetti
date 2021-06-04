#ifndef CaloHitContainer_h
#define CaloHitContainer_h

/** simulator includes **/
#include "CaloHit/CaloHit.h"
#include "GaugiKernel/DataVector.h"

namespace xAOD{
  // All objects inside of the data vector will be const
  typedef SG::DataVector<xAOD::CaloHit> CaloHitContainer;
}
#endif
