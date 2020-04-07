#ifndef RawCellContainer_h
#define RawCellContainer_h

/** simulator includes **/
#include "CaloCell/RawCell.h"
#include "GaugiKernel/DataVector.h"

namespace xAOD{
  // All objects inside of the data vector will be const
  typedef SG::DataVector<xAOD::RawCell> RawCellContainer;
}
#endif
