#ifndef ElectronMaker_h
#define ElectronMaker_h

#include "CaloCell/enumeration.h"
#include "GaugiKernel/StatusCode.h"
#include "GaugiKernel/DataHandle.h"
#include "GaugiKernel/Algorithm.h"
#include "CaloCell/CaloCellContainer.h"
#include "CaloCluster/CaloClusterContainer.h"
#include "Particle/Electron.h"
#include "Particle/ElectronContainer.h"



class ElectronMaker : public Gaugi::Algorithm
{

  public:
    /** Constructor **/
    ElectronMaker( std::string );
    
    virtual ~ElectronMaker();
    
    virtual StatusCode initialize() override;

    /*! Book all histograms into the current storegate */
    virtual StatusCode bookHistograms( SG::EventContext &ctx ) const override;
    
    virtual StatusCode pre_execute( SG::EventContext &ctx ) const override;
    
    virtual StatusCode execute( SG::EventContext &ctx , const G4Step *step) const override;
    
    /*! Execute in ComponentAccumulator */
    virtual StatusCode execute( SG::EventContext &ctx , int /*evt*/ ) const override;
    
    virtual StatusCode post_execute( SG::EventContext &ctx ) const override;
    
    virtual StatusCode fillHistograms( SG::EventContext &ctx ) const override;
    
    virtual StatusCode finalize() override;

    bool compute(const xAOD::CaloCluster* cluster, std::string workingPoint) const;


  private:
    int m_outputLevel;  
    std::string m_clusterKey;
    std::string m_electronKey;
    
    std::vector<float> m_tightCuts;
    std::vector<float> m_mediumCuts;
    std::vector<float> m_looseCuts;
    std::vector<float> m_vlooseCuts;
    std::vector<float> m_secondLambdaCuts;
    std::vector<float> m_lateralMomCuts;
    std::vector<float> m_longMomCuts;
    std::vector<float> m_fracMaxCuts;
    std::vector<float> m_secondRCuts;
    std::vector<float> m_lambdaCenterCuts;
};

#endif




