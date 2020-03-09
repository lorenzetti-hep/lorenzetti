#ifndef CaloClusterMaker_h
#define CaloClusterMaker_h

/** simulator libs **/
#include "core/enumeration.h"
#include "core/StatusCode.h"
#include "core/EventContext.h"
#include "core/AlgTool.h"
#include "CaloCell/CaloCellContainer.h"
#include "CaloCluster/CaloClusterContainer.h"
#include "TruthParticle/TruthParticleContainer.h"
#include "EventInfo/EventInfoContainer.h"

/** standard libs **/
#include <string>


class CaloClusterMaker : public AlgTool
{

  public:
    /** Constructor **/
    CaloClusterMaker( std::string );
    virtual ~CaloClusterMaker();
    
    virtual StatusCode initialize() override;
    virtual StatusCode pre_execute( EventContext *ctx ) override;
    virtual StatusCode execute( EventContext * ) override;
    virtual StatusCode post_execute( EventContext *ctx ) override;
    virtual StatusCode fillHistograms( EventContext *ctx ) override;
    virtual StatusCode finalize() override;



    PRIMITIVE_SETTER_AND_GETTER( float, m_etaWindow, setEtaWindow, etaWindow );
    PRIMITIVE_SETTER_AND_GETTER( float, m_phiWindow, setPhiWindow, phiWindow );
    PRIMITIVE_SETTER_AND_GETTER( float, m_energyThreshold, setEnergyThreshold, energyThreshold );
    PRIMITIVE_SETTER_AND_GETTER( float, m_dR, setDR, dR );
    PRIMITIVE_SETTER_AND_GETTER( bool, m_forceTruthMatch, setForceTruthMatch, forceTruthMatch );
    

  private:
 

    
    void fillParticle( xAOD::TruthParticle *);
    
    void fillCluster( xAOD::CaloCluster *);

    void calculate( xAOD::TruthParticle *);
    
    void calculate( xAOD::CaloCluster *);
    
    float dR( xAOD::CaloCluster *, xAOD::TruthParticle *);
    
    float sumEnergy( xAOD::TruthParticle *, CaloSampling::CaloSample, unsigned eta_ncell=1000, unsigned phi_ncell=1000 );
    
    float sumEnergy( xAOD::CaloCluster *, CaloSampling::CaloSample, unsigned eta_ncell=1000, unsigned phi_ncell=1000 );
    
    float maxEnergy( xAOD::TruthParticle *, CaloSampling::CaloSample, const xAOD::CaloCell *&maxCell , bool exclude=false);
    
    float maxEnergy( xAOD::CaloCluster *, CaloSampling::CaloSample, const xAOD::CaloCell *&maxCell , bool exclude=false);
 
    std::vector<xAOD::CaloCluster*> getAllClusters();
    
    std::vector<xAOD::TruthParticle*> getAllParticles();
      
   

    
    float m_dR;
    float m_etaWindow;
    float m_phiWindow;
    float m_energyThreshold;
    bool m_forceTruthMatch;

    const xAOD::CaloCellContainer *m_caloCellContainer;
    const xAOD::EventInfoContainer *m_eventInfoContainer;


    std::string m_inputEventKey;
    std::string m_inputCaloKey;
    std::string m_outputTruthKey;
    std::string m_outputClusterKey;
};

#endif




