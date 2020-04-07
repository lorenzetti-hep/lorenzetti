#ifndef CaloRingerBuilder_h
#define CaloRingerBuilder_h

#include "CaloCell/enumeration.h"
#include "GaugiKernel/StatusCode.h"
#include "GaugiKernel/DataHandle.h"
#include "GaugiKernel/Algorithm.h"
#include "CaloCell/CaloCellContainer.h"
#include "CaloCluster/CaloClusterContainer.h"
#include "TruthParticle/TruthParticleContainer.h"


class CaloRingerBuilder : public Gaugi::Algorithm
{

  public:
    /** Constructor **/
    CaloRingerBuilder( std::string );
    
    virtual ~CaloRingerBuilder();
    
    virtual StatusCode initialize() override;

    virtual StatusCode pre_execute( SG::EventContext &ctx ) const override;
    
    virtual StatusCode execute( SG::EventContext &ctx , const G4Step *step) const override;
    
    virtual StatusCode post_execute( SG::EventContext &ctx ) const override;
    
    virtual StatusCode fillHistograms( SG::EventContext &ctx ) const override;
    
    virtual StatusCode finalize() override;



  private:
 
    
    const xAOD::CaloCell* maxCell( const xAOD::CaloCluster*   , CaloSampling::CaloSample ) const;
    const xAOD::CaloCell* maxCell( const xAOD::TruthParticle* , CaloSampling::CaloSample ) const;
    


    std::string m_clusterKey;
    std::string m_ringerKey;
    std::string m_truthKey;
    std::string m_histPath;






};

#endif




