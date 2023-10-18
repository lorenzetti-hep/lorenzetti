#ifndef PileupMerge_h
#define PileupMerge_h

#include "GaugiKernel/Algorithm.h"
#include "CaloCell/enumeration.h"
#include "CaloHit/CaloHitContainer.h"
#include "EventInfo/EventInfoContainer.h"
#include "TruthParticle/TruthParticleContainer.h"
//#include "CaloHit/CaloHitConverter.h"
//#include "EventInfo/EventInfoConverter.h"
#include "TRandom3.h"


class PileupMerge : public Gaugi::Algorithm
{

  public:
    /** Constructor **/
    PileupMerge( std::string );
    
    virtual ~PileupMerge();

    virtual StatusCode initialize() override;

    virtual StatusCode bookHistograms( SG::EventContext &ctx ) const override;
    
    virtual StatusCode pre_execute( SG::EventContext &ctx ) const override;
    
    virtual StatusCode execute( SG::EventContext &ctx , const G4Step *step) const override;
    
    virtual StatusCode execute( SG::EventContext &/*ctx*/ , int /*evt*/) const override;

    virtual StatusCode post_execute( SG::EventContext &ctx ) const override;
    
    virtual StatusCode fillHistograms( SG::EventContext &ctx ) const override;
    
    virtual StatusCode finalize() override;


  private:
 


    template <class T> void InitBranch(TTree* fChain, std::string branch_name, T* param) const;
    
    std::string m_inputHitsKey;
    std::string m_outputHitsKey;
    std::string m_inputEventKey;
    std::string m_outputEventKey;

    std::string m_inputFile;
    std::string m_ntupleName;

    int m_outputLevel;

    mutable TRandom3 m_rng;
    float m_seed;

};

#endif
