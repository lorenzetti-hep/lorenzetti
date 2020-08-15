#ifndef CaloNtupleMaker_h
#define CaloNtupleMaker_h

#include "CaloCell/enumeration.h"
#include "EventInfo/EventInfo.h"
#include "CaloRings/CaloRings.h"
#include "CaloCluster/CaloCluster.h"
#include "GaugiKernel/StatusCode.h"
#include "GaugiKernel/DataHandle.h"
#include "GaugiKernel/Algorithm.h"
#include "GaugiKernel/DataHandle.h"


class CaloNtupleMaker : public Gaugi::Algorithm
{

  public:
    /** Constructor **/
    CaloNtupleMaker( std::string );
    
    virtual ~CaloNtupleMaker();
    
    virtual StatusCode initialize() override;

    virtual StatusCode bookHistograms( SG::EventContext &ctx ) const override;
    
    virtual StatusCode pre_execute( SG::EventContext &ctx ) const override;
    
    virtual StatusCode execute( SG::EventContext &ctx , const G4Step *step) const override;
    
    virtual StatusCode post_execute( SG::EventContext &ctx ) const override;
    
    virtual StatusCode fillHistograms( SG::EventContext &ctx ) const override;
    
    virtual StatusCode finalize() override;



  private:
 
    bool match( SG::EventContext &ctx , std::string key, xAOD::seed_t seed, const xAOD::CaloCluster *&cl) const;
    bool match( SG::EventContext &ctx , std::string key, const xAOD::CaloCluster *cluster, const xAOD::CaloRings *&ringer ) const;
    float dR( float eta1, float phi1, float eta2, float phi2 ) const;
    template <class T> void InitBranch(TTree* fChain, std::string branch_name, T* param) const;
    void Fill( SG::EventContext &ctx , TTree *tree, xAOD::seed_t seed, int evt, float mu ) const;
    
    std::string m_ntupleName;
    std::string m_eventKey;
    std::string m_clusterKey;
    std::string m_truthClusterKey;
    std::string m_ringerKey;
    std::string m_truthRingerKey;
    bool m_dumpCells;
    int m_outputLevel;
    float m_deltaR;
};

#endif

