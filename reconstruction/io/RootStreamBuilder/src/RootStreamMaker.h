#ifndef RootStreamMaker_h
#define RootStreamMaker_h

#include "GaugiKernel/StatusCode.h"
#include "GaugiKernel/DataHandle.h"
#include "GaugiKernel/Algorithm.h"
#include "GaugiKernel/DataHandle.h"



class RootStreamMaker : public Gaugi::Algorithm
{

  public:
    /** Constructor **/
    RootStreamMaker( std::string );
    
    virtual ~RootStreamMaker();
    
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
    
    std::vector<std::string> m_containers;
    std::string m_ntupleName;
    float m_etaWindow;
    float m_phiWindow;
    bool m_onlyRoI;
    int m_outputLevel;
};

#endif

