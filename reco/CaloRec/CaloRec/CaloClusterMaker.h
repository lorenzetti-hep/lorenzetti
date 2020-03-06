#ifndef CaloClusterMaker_h
#define CaloClusterMaker_h

/** simulator libs **/
#include "core/enumeration.h"
#include "core/StatusCode.h"
#include "core/EventContext.h"
#include "core/AlgTool.h"
#include "CaloCell/CaloCellContainer.h"
#include "CaloCluster/CaloClusterContainer.h"

/** standard libs **/
#include <string>


class CaloClusterMaker : public AlgTool
{

  public:
    /** Constructor **/
    CaloClusterMaker( std::string );
    /** Destructor **/
    virtual ~CaloClusterMaker();
    /** initialize the object **/
    virtual StatusCode initialize() override;
    /** pre execute the event before step action **/
    virtual StatusCode pre_execute( EventContext *ctx ) override;
    /** override to avoid abstract python import problem **/ 
    virtual StatusCode execute( EventContext * ) override{return ErrorCode::SUCCESS;};
    /** post execute the event after step action **/
    virtual StatusCode post_execute( EventContext *ctx ) override;
    /** fill histograms after post execute **/
    virtual StatusCode fillHistograms( EventContext *ctx ) override;
    /** override to avoid abstract python import problem **/ 
    virtual StatusCode finalize() override {return ErrorCode::SUCCESS;};



    PRIMITIVE_SETTER_AND_GETTER( float, m_etaWindow, setEtaWindow, etaWindow );
    PRIMITIVE_SETTER_AND_GETTER( float, m_phiWindow, setPhiWindow, phiWindow );
    PRIMITIVE_SETTER_AND_GETTER( float, m_energyThreshold, setEnergyThreshold, energyThreshold );
    

  private:
 
    void fillClusters( std::vector< xAOD::CaloCluster* > vec_cluster,  const xAOD::CaloCellContainer *container );

    std::vector < xAOD::CaloCluster* > getAllClusters( const xAOD::CaloCellContainer *container , const xAOD::TruthContainer *truthContainer );
    
    void calculate( std::vector< xAOD::CaloCluster* > vec_cluster );

    float sumEnergy( xAOD::CaloCluster *, CaloSampling::CaloSample, unsigned eta_ncell=1000, unsigned phi_ncell=1000 );
    
    float maxEnergy( xAOD::CaloCluster *, CaloSampling::CaloSample, const xAOD::CaloCell *maxCell , bool exclude=false);
      
    // eta window
    float m_etaWindow;
    // phi window
    float m_phiWindow;
    // the energy threshold to accept a RoI center
    float m_energyThreshold;

    bool m_truthMatch;
};

#endif




