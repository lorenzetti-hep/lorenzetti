#ifndef CaloCellContainer_h
#define CaloCellContainer_h

/** simulator includes **/
#include "CaloCell/CaloCell.h"
#include "GaugiKernel/DataVector.h"

namespace xAOD{
  // All objects inside of the data vector will be const
  typedef SG::DataVector<xAOD::CaloCell> CaloCellContainer;
}
#endif
