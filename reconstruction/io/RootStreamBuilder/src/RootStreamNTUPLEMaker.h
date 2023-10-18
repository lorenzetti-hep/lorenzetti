#ifndef RootStreamNTUPLEMaker_h
#define RootStreamNTUPLEMaker_h

#include "GaugiKernel/Algorithm.h"
#include "CaloCell/enumeration.h"
#include "EventInfo/EventInfo.h"
#include "TruthParticle/TruthParticle.h"
#include "CaloCluster/CaloCluster.h"
#include "CaloRings/CaloRings.h"




class RootStreamNTUPLEMaker : public Gaugi::Algorithm
{

  public:
    /** Constructor **/
    RootStreamNTUPLEMaker( std::string );
    
    virtual ~RootStreamNTUPLEMaker();

    virtual StatusCode initialize() override;

    virtual StatusCode bookHistograms( SG::EventContext &ctx ) const override;
    
    virtual StatusCode pre_execute( SG::EventContext &ctx ) const override;
    
    virtual StatusCode execute( SG::EventContext &ctx , const G4Step *step) const override;
    
    virtual StatusCode execute( SG::EventContext &ctx , int evt ) const override;

    virtual StatusCode post_execute( SG::EventContext &ctx ) const override;
    
    virtual StatusCode fillHistograms( SG::EventContext &ctx ) const override;
    
    virtual StatusCode finalize() override;


  private:
 
    StatusCode deserialize( int evt, SG::EventContext &ctx ) const;

    template <class T> void InitBranch(TTree* fChain, std::string branch_name, T* param) const;
    
    std::string m_cellsKey;
    std::string m_eventKey;
    std::string m_truthKey;
    std::string m_clusterKey;
    std::string m_ringerKey;
    std::string m_inputFile;
    std::string m_ntupleName;
    std::string m_outputNtupleName;


    int m_outputLevel;

};

#endif

