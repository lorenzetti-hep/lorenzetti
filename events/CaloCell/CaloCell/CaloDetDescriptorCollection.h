#ifndef CaloDetDescriptorCollection_h
#define CaloDetDescriptorCollection_h

/** simulator includes **/
#include "CaloCell/CaloDetDescriptor.h"
#include "GaugiKernel/Collection.h"

namespace xAOD{
  // All objects inside of the data vector will be const
  typedef SG::Collection<xAOD::CaloDetDescriptor> CaloDetDescriptorCollection;
}
#endif
