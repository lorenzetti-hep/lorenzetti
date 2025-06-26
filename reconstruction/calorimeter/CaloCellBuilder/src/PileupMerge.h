#ifndef PileupMerge_h
#define PileupMerge_h

#include "GaugiKernel/Algorithm.h"
#include "CaloCell/enumeration.h"
#include "CaloHit/CaloHitContainer.h"
#include "EventInfo/EventInfoContainer.h"
#include "TruthParticle/TruthParticleContainer.h"
#include "CaloHit/CaloHitConverter.h"
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
 


    template <class T> TBranch* InitBranch(TTree* fChain, std::string branch_name, T* param) const;
    int poisson(double nAvg) const;
    void Read( SG::EventContext &ctx, const std::vector<std::string> &paths, std::string name ) const;


    void allocate( SG::ReadHandle<xAOD::CaloHitContainer> &container , std::vector<xAOD::CaloHit*> &vec_hits ) const;
    void deallocate( std::vector<xAOD::CaloHit*> &vec_hits ) const;
    float merge( SG::EventContext &ctx, std::vector<xAOD::CaloHit*> &vec_hits ) const;



    std::string m_inputHitsKey;
    std::string m_outputHitsKey;
    std::string m_inputEventKey;
    std::string m_outputEventKey;

    int m_outputLevel;
    mutable TRandom3 m_rng;
    float m_pileupAvg;
    float m_pileupSigma;
    float m_seed;
    int   m_maxRetry;
    float m_trunc_mu;
    /*! The start bunch crossing id for energy estimation */
    int m_bcid_start;
    /*! The end bunch crossing id for energy estimation */
    int m_bcid_end;

    std::vector<std::string> m_lowPileupInputFiles;
    std::vector<std::string> m_highPileupInputFiles;
    std::string m_ntupleName;



};

#endif
