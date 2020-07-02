#ifndef ShowerShapes_h
#define ShowerShapes_h

#include "CaloTool.h"
#include "CaloCluster/CaloClusterContainer.h"
#include "CaloCell/CaloCell.h"

class ShowerShapes : public CaloTool
{

  public:
    /** Constructor **/
    ShowerShapes( std::string );
    virtual ~ShowerShapes()=default;
    
    virtual StatusCode initialize() override;
    virtual StatusCode finalize() override;

    virtual StatusCode executeTool( const xAOD::EventInfo *, xAOD::RawCell * ) const override;
    virtual StatusCode executeTool( const xAOD::EventInfo *, xAOD::CaloCell * ) const override;
    virtual StatusCode executeTool( const xAOD::EventInfo *, xAOD::CaloCluster * ) const override;
    virtual StatusCode executeTool( const xAOD::EventInfo *, xAOD::TruthParticle * ) const override;


  private:
 

    float calculateWeta2( xAOD::CaloCluster *clus , CaloSampling::CaloSample sampling, unsigned eta_ncell=3, unsigned phi_ncell=5 ) const;
    float sumEnergy( xAOD::CaloCluster *, CaloSampling::CaloSample, unsigned eta_ncell=1000, unsigned phi_ncell=1000 ) const;
};

#endif




