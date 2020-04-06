#ifndef ICaloCellTool_h
#define ICaloCellTool_h

#include "GaugiKernel/AlgTool.h"
#include "CaloCell/CaloCell.h"

class ICaloCellTool : public Gaugi::AlgTool
{

  public:
    /** Constructor **/
    ICaloCellTool( std::string name ) : Gaugi::AlgTool(name){};
    
    virtual StatusCode executeTool( xAOD::CaloCell * ) const = 0;

};

#endif




