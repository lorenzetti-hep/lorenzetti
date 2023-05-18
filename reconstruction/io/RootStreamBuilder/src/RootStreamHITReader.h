#ifndef RootStreamHITReader_h
#define RootStreamHITReader_h

#include "GaugiKernel/Algorithm.h"
#include "CaloCell/enumeration.h"
#include "EventInfo/EventInfo.h"
#include "TruthParticle/TruthParticle.h"


class RootStreamHITReader : public Gaugi::Algorithm
{

  public:
    /** Constructor **/
    RootStreamHITReader( std::string );
    
    virtual ~RootStreamHITReader();
    
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
    
    std::string m_hitsKey;
    std::string m_eventKey;
    std::string m_truthKey;
    std::string m_inputFile;
    std::string m_ntupleName;
    int m_outputLevel;
};

#endif

