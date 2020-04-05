#ifndef EnergyEstimatorTool_h
#define EnergyEstimatorTool_h

#include "ICaloCellTool.h"



class EnergyEstimatorTool : public ICaloCellTool
{

  public:
    /** Constructor **/
    EnergyEstimatorTool( std::string );
    virtual ~EnergyEstimatorTool();
    virtual StatusCode initialize() override;
    virtual StatusCode finalize() override;
    virtual StatusCode executeTool( CaloCell * ) const override;


  private:
 
};

#endif




