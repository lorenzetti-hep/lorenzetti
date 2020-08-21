#ifndef ShowerShapes_h
#define ShowerShapes_h

#include "GaugiKernel/AlgTool.h"
#include "GaugiKernel/EDM.h"
#include "CaloCluster/CaloClusterContainer.h"
#include "CaloCell/CaloCell.h"

class ShowerShapes : public Gaugi::AlgTool
{

  public:
    /** Constructor **/
    ShowerShapes( std::string );
    virtual ~ShowerShapes()=default;
    
    virtual StatusCode initialize() override;
    virtual StatusCode finalize() override;

    virtual StatusCode executeTool( const xAOD::EventInfo *, Gaugi::EDM * ) const override;


  private:
 

    float calculateWeta2( xAOD::CaloCluster * , CaloSampling::CaloLayer, unsigned eta_ncell=3, unsigned phi_ncell=5 ) const;
    float sumEnergy( xAOD::CaloCluster *, CaloSampling::CaloLayer, unsigned eta_ncell=1000, unsigned phi_ncell=1000 ) const;
};

#endif




