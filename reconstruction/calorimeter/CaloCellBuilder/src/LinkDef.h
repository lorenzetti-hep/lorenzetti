
#include "src/CaloCellMaker.h"
#include "src/CaloCellMerge.h"
#include "src/PulseGenerator.h"
#include "src/OptimalFilter.h"
//#include "src/CrossTalkMaker.h"
#include "src/CaloHitMaker.h"
#include "src/CaloHitMerge.h"
#include "src/PileupMerge.h"
#include "src/ConstrainedOptimalFilter.h"

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ class CaloCellMaker+;
#pragma link C++ class CaloCellMerge+;
#pragma link C++ class PulseGenerator+;
#pragma link C++ class OptimalFilter+;
//#pragma link C++ class CrossTalkMaker+;
#pragma link C++ class CaloHitMaker+;
#pragma link C++ class CaloHitMerge+;
#pragma link C++ class ConstrainedOptimalFilter++;
#pragma link C++ class PileupMerge+;
#endif
