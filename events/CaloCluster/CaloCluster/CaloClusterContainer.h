#ifndef CaloClusterContainer_h
#define CaloClusterContainer_h

/** simulator includes **/
#include "CaloCluster/CaloCluster.h"
#include "GaugiKernel/DataVector.h"

namespace xAOD{
  typedef DataVector<const xAOD::CaloCluster> CaloClusterContainer;
}
#endif
