#ifndef ICaloCellTool_h
#define ICaloCellTool_h

#include "GaugiKernel/AlgTool.h"


class ICaloCellTool : public AlgTool
{

  public:
    /** Constructor **/
    ICaloCellTool( std::string name) : AlgTool(name){};
    
    virtual StatusCode executeTool( CaloCell * ) const = 0;

};

#endif




