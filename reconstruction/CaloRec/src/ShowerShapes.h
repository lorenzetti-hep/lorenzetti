#ifndef ShowerShapes_h
#define ShowerShapes_h

#include "CaloRecTool.h"
#include "CaloCluster/CaloClusterContainer.h"
#include "TruthParticle/TruthParticleContainer.h"


class ShowerShapes : public CaloRecTool
{

  public:
    /** Constructor **/
    ShowerShapes( std::string );
    virtual ~ShowerShapes();
    
    virtual StatusCode initialize() override;
    virtual StatusCode finalize() override;

    virtual StatusCode executeTool( xAOD::CaloCell * ) const override;
    virtual StatusCode executeTool( xAOD::CaloCluster * ) const override;
    virtual StatusCode executeTool( xAOD::TruthParticle * ) const override;


  private:
 
    float sumEnergy( xAOD::TruthParticle *, CaloSampling::CaloSample, unsigned eta_ncell=1000, unsigned phi_ncell=1000 ) const;
    float sumEnergy( xAOD::CaloCluster *, CaloSampling::CaloSample, unsigned eta_ncell=1000, unsigned phi_ncell=1000 ) const;
 
};

#endif




