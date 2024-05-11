#ifndef ShowerShapes_h
#define ShowerShapes_h
#include "GaugiKernel/AlgTool.h"
#include "GaugiKernel/EDM.h"
#include "CaloCluster/CaloClusterContainer.h"
#include "CaloCell/CaloCell.h"

#include <cmath>
#include <Math/GenVector/Cartesian3D.h>
#include <TVector3.h>
#include <TMatrixD.h>
#include <TMatrixDEigen.h>
class ShowerShapes : public Gaugi::AlgTool
{

  public:
    /** Constructor **/
    ShowerShapes( std::string );
    virtual ~ShowerShapes()=default;
    
    virtual StatusCode initialize() override;
    virtual StatusCode finalize() override;
    virtual StatusCode execute( SG::EventContext &ctx, Gaugi::EDM * ) const override;
  
  private:
 
    float calculateWeta2( xAOD::CaloCluster * , unsigned eta_ncell=3, unsigned phi_ncell=5 ) const;
    float sumEnergyEM( xAOD::CaloCluster *, int sampling, unsigned eta_ncell=1000, unsigned phi_ncell=1000 ) const;
    float sumEnergyHAD( xAOD::CaloCluster *, int sampling, unsigned eta_ncell=1000, unsigned phi_ncell=1000 ) const;
	std::vector<TVector3> calculateShowerAxis(xAOD::CaloCluster *) const;
    float calculateSecondR (xAOD::CaloCluster *, std::vector<TVector3>) const;
    float calculateSecondLambda (xAOD::CaloCluster *, std::vector<TVector3>) const;
    float calculateLambdaCenter (xAOD::CaloCluster *, std::vector<TVector3>) const;
    float calculateFracMax(xAOD::CaloCluster *, std::vector<TVector3>) const;
	float calculateLateralMom(xAOD::CaloCluster *, std::vector<TVector3>) const;
	float calculateLongitudinalMom(xAOD::CaloCluster *, std::vector<TVector3>) const;


};

#endif