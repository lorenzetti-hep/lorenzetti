#ifndef ElectronContainer_h
#define ElectronContainer_h

/** simulator includes **/
#include "Particle/Electron.h"
#include "GaugiKernel/DataVector.h"

namespace xAOD{
  typedef SG::DataVector<xAOD::Electron> ElectronContainer;
}
#endif
