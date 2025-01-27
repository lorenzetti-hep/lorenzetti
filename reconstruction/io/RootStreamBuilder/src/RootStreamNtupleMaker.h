#ifndef RootStreamNtupleMaker_h
#define RootStreamNtupleMaker_h

#include "GaugiKernel/Algorithm.h"
#include "CaloCell/enumeration.h"
#include "EventInfo/EventInfo.h"
#include "TruthParticle/TruthParticle.h"
#include "CaloCluster/CaloCluster.h"
#include "CaloRings/CaloRings.h"
#include "Egamma/Electron.h"




class RootStreamNtupleMaker : public Gaugi::Algorithm
{

  public:
    /** Constructor **/
    RootStreamNtupleMaker( std::string );
    
    virtual ~RootStreamNtupleMaker();

    virtual StatusCode initialize() override;

    virtual StatusCode bookHistograms( SG::EventContext &ctx ) const override;
    
    virtual StatusCode pre_execute( SG::EventContext &ctx ) const override;
    
    virtual StatusCode execute( SG::EventContext &ctx , const G4Step *step) const override;
    
    virtual StatusCode execute( SG::EventContext &ctx , int evt ) const override;

    virtual StatusCode post_execute( SG::EventContext &ctx ) const override;
    
    virtual StatusCode fillHistograms( SG::EventContext &ctx ) const override;
    
    virtual StatusCode finalize() override;

  private:
 
    template <class T> void InitBranch(TTree* fChain, std::string branch_name, T* param) const;
    

    std::string m_seedsKey;
    std::string m_eventKey;
    std::string m_truthKey;
    std::string m_clusterKey;
    std::string m_ringerKey;
    std::string m_electronKey;
    std::string m_outputNtupleName;
  

    int m_outputLevel;

};

#endif

