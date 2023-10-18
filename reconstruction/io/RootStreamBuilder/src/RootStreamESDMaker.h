#ifndef RootStreamESDMaker_h
#define RootStreamESDMaker_h

#include "CaloCell/enumeration.h"
#include "EventInfo/EventInfo.h"
#include "EventInfo/EventSeed.h"
#include "TruthParticle/TruthParticle.h"
#include "GaugiKernel/StatusCode.h"
#include "GaugiKernel/DataHandle.h"
#include "GaugiKernel/Algorithm.h"
#include "GaugiKernel/DataHandle.h"



class RootStreamESDMaker : public Gaugi::Algorithm
{

  public:
    /** Constructor **/
    RootStreamESDMaker( std::string );
    
    virtual ~RootStreamESDMaker();
    
    virtual StatusCode initialize() override;

    virtual StatusCode bookHistograms( SG::EventContext &ctx ) const override;
    
    virtual StatusCode pre_execute( SG::EventContext &ctx ) const override;
    
    virtual StatusCode execute( SG::EventContext &ctx , const G4Step *step) const override;
    
    virtual StatusCode execute( SG::EventContext &ctx , int /*evt*/ ) const override;

    virtual StatusCode post_execute( SG::EventContext &ctx ) const override;
    
    virtual StatusCode fillHistograms( SG::EventContext &ctx ) const override;
    
    virtual StatusCode finalize() override;



  private:
 

    StatusCode serialize( SG::EventContext &ctx ) const;

    template <class T> void InitBranch(TTree* fChain, std::string branch_name, T* param) const;
    
    std::string m_ntupleName;
    std::string m_inputCellsKey;
    std::string m_inputEventKey;
    std::string m_inputTruthKey;
    std::string m_inputSeedsKey;
    std::string m_outputCellsKey;
    std::string m_outputEventKey;
    std::string m_outputTruthKey;
    std::string m_outputSeedsKey;
    
    float m_etaWindow, m_phiWindow;

    int m_outputLevel;
};

#endif

