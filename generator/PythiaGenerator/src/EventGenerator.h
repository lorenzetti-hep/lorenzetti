#ifndef EventGenerator_h
#define EventGenerator_h

#include "Pythia8/Pythia.h"
#include "Pythia8/HeavyIons.h"
#include "Pythia8Plugins/HepMC2.h"
#include "GaugiKernel/Property.h"
#include "GaugiKernel/MsgStream.h"
#include "GaugiKernel/StoreGate.h"
#include "GaugiKernel/StatusCode.h"
#include "EventInfo/EventInfo.h"
#include "tools/ParticleFilter.h"
#include "TTree.h"
#include <exception>

class NotInterestingEvent: public std::exception
{
  virtual const char* what() const throw()
  {
    return "Not interesting event";
  }
};


class AbortPrematurely: public std::exception
{
  virtual const char* what() const throw()
  {
    return "Abort prematurely";
  }
};


class Physics: public MsgService,
               public Gaugi::PropertyService
{
  public:
    Physics(): PropertyService() {};
    ~Physics()=default;
    virtual StatusCode initialize()=0;
    virtual StatusCode run( std::vector<xAOD::seed_t>&, std::vector<std::vector<Pythia8::Particle*>> &)=0;
    virtual StatusCode finalize()=0;
};



class EventGenerator: public MsgService, 
                      public Gaugi::PropertyService
{
  public:

    EventGenerator();
    ~EventGenerator();
    StatusCode initialize();
    StatusCode run();
    StatusCode finalize();

    void push_back( Physics *tool ){ m_tools.push_back(tool); };

  private:
 
    /*! Create the main event into the output ntuple */
    StatusCode generate( Physics *, std::vector<xAOD::seed_t> &);

    /*! generate pileup around the seed and add it into the ntuple */
    StatusCode addPileup( std::vector<xAOD::seed_t> );

    /*! Clear the ntuple */
    void clear();
    /*! Poisson random number generation*/ 
    int poisson(double nAvg, Pythia8::Rndm& rndm);


    std::vector<Physics*> m_tools;

    TTree *m_tree;
    Pythia8::Pythia m_mb_pythia;
    SG::StoreGate *m_store;

    bool m_useWindow;

    int m_seed;
    int m_select;
    int m_bc_id_start;
    int m_bc_id_end;
    int m_outputLevel;
    float m_etaMax;
    float m_nPileupAvg;
    float m_mb_delta_eta;
    float m_mb_delta_phi;
    float m_sigma_t;
    float m_sigma_z;
     
    float m_nEvent;
    float m_nAbort;
    std::string m_outputFile;
    std::string m_minbiasFile;
  
    /*! Ntuple output */
    float m_avg_mu;
    std::vector<int>   *m_p_isMain    ;
    std::vector<int>   *m_bc_id_nhits ;
    std::vector<int>   *m_p_pdg_id    ; 
    std::vector<int>   *m_p_bc_id     ; 
    std::vector<float> *m_bc_id_mu    ; 
    std::vector<float> *m_p_px        ; 
    std::vector<float> *m_p_py        ; 
    std::vector<float> *m_p_pz        ;
    std::vector<float> *m_p_prod_x    ; 
    std::vector<float> *m_p_prod_y    ; 
    std::vector<float> *m_p_prod_z    ;  
    std::vector<float> *m_p_prod_t    ;
    std::vector<float> *m_p_eta       ; 
    std::vector<float> *m_p_phi       ;
    std::vector<float> *m_p_e         ;  
    std::vector<float> *m_p_et        ;
};

#endif










