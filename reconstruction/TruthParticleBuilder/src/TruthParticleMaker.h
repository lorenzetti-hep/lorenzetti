#ifndef TruthParticleMaker_h
#define TruthParticleMaker_h

#include "CaloCell/enumeration.h"
#include "GaugiKernel/StatusCode.h"
#include "GaugiKernel/DataHandle.h"
#include "GaugiKernel/Algorithm.h"
#include "TruthParticle/TruthParticleContainer.h"
#include "EventInfo/EventInfoContainer.h"


class TruthParticleMaker : public Gaugi::Algorithm
{

  public:
    /** Constructor **/
    TruthParticleMaker( std::string );
    
    virtual ~TruthParticleMaker();
    
    virtual StatusCode initialize() override;

    /*! Book all histograms into the current storegate **/
    virtual StatusCode bookHistograms( SG::EventContext &ctx ) const override;
    
    virtual StatusCode pre_execute( SG::EventContext &ctx ) const override;
    
    virtual StatusCode execute( SG::EventContext &ctx , const G4Step *step) const override;
    
    virtual StatusCode post_execute( SG::EventContext &ctx ) const override;
    
    virtual StatusCode fillHistograms( SG::EventContext &ctx ) const override;
    
    virtual StatusCode finalize() override;



  private:
 
    
    // input keys
    std::string m_eventKey;
    
    // output keys
    std::string m_truthKey;

    std::string m_histPath;

};

#endif




