#ifndef RawNtupleMaker_h
#define RawNtupleMaker_h

#include "CaloCell/enumeration.h"
#include "EventInfo/EventInfo.h"
#include "CaloRings/CaloRings.h"
#include "CaloCluster/CaloCluster.h"
#include "GaugiKernel/StatusCode.h"
#include "GaugiKernel/DataHandle.h"
#include "GaugiKernel/Algorithm.h"
#include "GaugiKernel/DataHandle.h"

struct raw_cell_t {
  float eta;
  float phi;
  float deta;
  float dphi;
  int bcid_start;
  int bcid_end;
  int bc_nsamples;
  float bc_duration;
  std::vector<float> pulse;
  std::vector<std::vector<float>> pulsePerBunch;
  std::vector<float> rawEnergySamples;
  int sampling;
};



class RawNtupleMaker : public Gaugi::Algorithm
{

  public:
    /** Constructor **/
    RawNtupleMaker( std::string );
    
    virtual ~RawNtupleMaker();
    
    virtual StatusCode initialize() override;

    virtual StatusCode bookHistograms( SG::StoreGate &store ) const override;
    
    virtual StatusCode pre_execute( SG::EventContext &ctx ) const override;
    
    virtual StatusCode execute( SG::EventContext &ctx , const G4Step *step) const override;
    
    virtual StatusCode post_execute( SG::EventContext &ctx ) const override;
    
    virtual StatusCode fillHistograms( SG::EventContext &ctx, SG::StoreGate &store ) const override;
    
    virtual StatusCode finalize() override;



  private:
 
    void Fill( SG::EventContext &ctx , TTree *tree  ) const;
    template <class T> void InitBranch(TTree* fChain, std::string branch_name, T* param) const;
  
    float dR( float eta1, float phi1, float eta2, float phi2 ) const;
    float m_etaWindow;
    float m_phiWindow;
    std::string m_ntupleName;
    std::string m_eventKey;
    std::string m_cellsKey;
    int m_outputLevel;
};

#endif




