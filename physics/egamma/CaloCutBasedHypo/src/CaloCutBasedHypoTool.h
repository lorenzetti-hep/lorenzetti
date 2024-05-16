#ifndef CaloCutBasedHypoTool_h
#define CaloCutBasedHypoTool_h

#include "CaloCell/enumeration.h"
#include "GaugiKernel/StatusCode.h"
#include "GaugiKernel/DataHandle.h"
#include "GaugiKernel/Algorithm.h"
#include "CaloCell/CaloCellContainer.h"
#include "CaloCluster/CaloClusterContainer.h"
#include "Particle/Electron.h"
#include "Particle/ElectronContainer.h"



class CaloCutBasedHypoTool : public Gaugi::Algorithm
{

  public:
    /** Constructor **/
    CaloCutBasedHypoTool( std::string );
    
    virtual ~CaloCutBasedHypoTool();
    
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

    bool computeDecision(const xAOD::CaloCluster* cluster, std::string workingPoint) const;


  private:

    std::string m_clusterKey;
    std::string m_electronKey;
    int m_outputLevel;  
    std::vector<float> m_tightCuts;
    std::vector<float> m_mediumCuts;
    std::vector<float> m_looseCuts;
    std::vector<float> m_vlooseCuts;

};

#endif




