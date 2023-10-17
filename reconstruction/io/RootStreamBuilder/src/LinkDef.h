
#include "src/RootStreamMaker.h"
#include "src/RootStreamReader.h"

#include "src/converter/CaloClusterConverter.h"
#include "src/converter/CaloCellConverter.h"
#include "src/converter/CaloHitConverter.h"
#include "src/converter/CaloRingsConverter.h"
#include "src/converter/EventInfoConverter.h"
#include "src/converter/EventSeedConverter.h"
#include "src/converter/TruthParticleConverter.h"

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;

#pragma link C++ class RootStreamMaker+;
#pragma link C++ class RootStreamReader+;


#pragma link C++ struct xAOD::TruthParticle_t+;
#pragma link C++ class std::vector< xAOD::TruthParticle_t >+;
#pragma link C++ struct xAOD::EventInfo_t+;
#pragma link C++ class std::vector< xAOD::EventInfo_t >+;
#pragma link C++ struct xAOD::EventSeed_t+;
#pragma link C++ class std::vector< xAOD::EventSeed_t >+;
#pragma link C++ struct xAOD::CaloRings_t+;
#pragma link C++ class std::vector< xAOD::CaloRings_t >+;
#pragma link C++ struct xAOD::CaloHit_t+;
#pragma link C++ class std::vector< xAOD::CaloHit_t >+;
#pragma link C++ struct xAOD::CaloCell_t+;
#pragma link C++ class std::vector< xAOD::CaloCell_t >+;
#pragma link C++ struct xAOD::CaloDetDescriptor_t+;
#pragma link C++ class std::vector< xAOD::CaloDetDescriptor_t >+;


#endif
