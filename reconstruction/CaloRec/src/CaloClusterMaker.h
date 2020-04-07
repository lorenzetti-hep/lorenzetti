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
#include "ShowerShapes.h"


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
 
    
    template<class T> void fillCluster( SG::EventContext &ctx,  T* clus ) const;
    
    float dR( const xAOD::CaloCluster *, const xAOD::TruthParticle *) const;
 
    std::vector<xAOD::CaloCluster*> getAllClusters( SG::EventContext &ctx ) const;
    
    std::vector<xAOD::TruthParticle*> getAllParticles( SG::EventContext &ctx ) const;
      
    
    float m_energyThreshold;
    std::vector<std::string> m_collectionKeys;
    std::string m_clusterKey;
    std::string m_eventKey;
    std::string m_truthKey;
    float m_etaWindow;
    float m_phiWindow;
    float m_dR;
    bool m_forceTruthMatch;
    std::string m_histPath;


    ShowerShapes *m_showerShapes;




};

#endif




