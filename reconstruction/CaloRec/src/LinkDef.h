
#include "src/CaloTool.h"
#include "src/CaloCellMaker.h"
#include "src/CaloCellMerge.h"
#include "src/CaloClusterMaker.h"
#include "src/CaloNtupleMaker.h"
#include "src/RawNtupleMaker.h"
#include "src/PulseGenerator.h"
#include "src/OptimalFilter.h"


#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;


#pragma link C++ class CaloTool+;
#pragma link C++ class CaloCellMaker+;
#pragma link C++ class CaloNtupleMaker+;
#pragma link C++ class RawNtupleMaker+;
#pragma link C++ class CaloCellMerge+;
#pragma link C++ class CaloClusterMaker+;
#pragma link C++ class PulseGenerator+;
#pragma link C++ class OptimalFilter+;
#pragma link C++ struct cell_t+;
#pragma link C++ class std::vector< cell_t >+;

#endif
