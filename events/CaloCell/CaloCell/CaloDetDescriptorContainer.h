#ifndef CaloDetDescriptorContainer_h
#define CaloDetDescriptorContainer_h

/** simulator includes **/
#include "CaloCell/CaloDetDescriptor.h"
#include "GaugiKernel/DataVector.h"

namespace xAOD{
  // All objects inside of the data vector will be const
  typedef SG::DataVector<xAOD::CaloDetDescriptor> CaloDetDescriptorContainer;
}
#endif
