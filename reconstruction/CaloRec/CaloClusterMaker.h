#ifndef CaloClusterMaker_h
#define CaloClusterMaker_h

#include "CaloCell/enumeration.h"
#include "GaugiKernel/StatusCode.h"
#include "GaugiKernel/DataHandle.h"
#include "GaugiKernel/Algorithm.h"
#include "CaloCell/CaloCellContainer.h"
#include "CaloCluster/CaloClusterContainer.h"
#include "TruthParticle/TruthParticleContainer.h"
#include "EventInfo/EventInfoContainer.h"



class CaloClusterMaker : public Gaugi::Algorithm
{

  public:
    /** Constructor **/
    CaloClusterMaker( std::string );
    
    virtual ~CaloClusterMaker();
    
    virtual StatusCode initialize() override;

    virtual StatusCode pre_execute( SG::EventContext &ctx ) const override;
    
    virtual StatusCode execute( SG::EventContext &ctx , const G4Step *step) const override;
    
    virtual StatusCode post_execute( SG::EventContext &ctx ) const override;
    
    virtual StatusCode fillHistograms( SG::EventContext &ctx ) const override;
    
    virtual StatusCode finalize() override;



  private:
 
    
    template<class T> void fillCluster( T* clus ) const;
    
    float dR( const xAOD::CaloCluster *, const xAOD::TruthParticle *) const;
 
    std::vector<xAOD::CaloCluster*> getAllClusters( SG::EventContext &ctx ) const;
    
    std::vector<xAOD::TruthParticle*> getAllParticles( SG::EventContext &ctx ) const;
      
    
    float m_dR;
    float m_etaWindow;
    float m_phiWindow;
    float m_energyThreshold;
    bool m_forceTruthMatch;



    std::string m_inputEventKey;
    std::string m_inputCaloKey;
    std::string m_outputTruthKey;
    std::string m_outputClusterKey;
};

#endif




