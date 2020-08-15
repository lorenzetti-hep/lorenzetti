#ifndef CaloRingerBuilder_h
#define CaloRingerBuilder_h

#include "CaloCell/enumeration.h"
#include "GaugiKernel/StatusCode.h"
#include "GaugiKernel/DataHandle.h"
#include "GaugiKernel/Algorithm.h"
#include "CaloRings/CaloRingsContainer.h"
#include "CaloCluster/CaloClusterContainer.h"


class CaloRingerBuilder : public Gaugi::Algorithm
{

  public:
    /** Constructor **/
    CaloRingerBuilder( std::string );
    
    virtual ~CaloRingerBuilder();
    
    virtual StatusCode initialize() override;

    virtual StatusCode bookHistograms( SG::EventContext &ctx ) const override;
    
    virtual StatusCode pre_execute( SG::EventContext &ctx ) const override;
    
    virtual StatusCode execute( SG::EventContext &ctx , const G4Step *step) const override;
    
    virtual StatusCode post_execute( SG::EventContext &ctx ) const override;
    
    virtual StatusCode fillHistograms( SG::EventContext &ctx ) const override;
    
    virtual StatusCode finalize() override;



  private:
 
    int m_maxRingSets;    
    int m_maxRingsAccumulated;

    const xAOD::CaloCell* maxCell( const xAOD::CaloCluster*   , CaloSampling::CaloSample ) const;
   
    std::string m_histPath;

    std::string m_clusterKey;
    std::string m_ringerKey;

    std::vector<float>  m_detaRings;
    std::vector<float>  m_dphiRings;
    std::vector<int>    m_nRings;
    std::vector<int>    m_layerRings;

    int m_outputLevel;
};



#endif




