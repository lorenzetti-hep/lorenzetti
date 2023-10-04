#ifndef GenKernel_EventTape_h
#define GenKernel_EventTape_h


#include "GaugiKernel/MsgStream.h"
#include "GaugiKernel/StoreGate.h"
#include "GaugiKernel/StatusCode.h"
#include "GaugiKernel/Property.h"
#include "GenKernel/exceptions.h"
#include "GenKernel/IAlgorithm.h"
#include "GenKernel/Event.h"

#include "TTree.h"
#include <vector>
#include <string>


namespace generator{

  class EventTape: public MsgService, 
                   public Gaugi::PropertyService
  {
    public:
  
      EventTape();
      ~EventTape();
  
      StatusCode initialize();
      StatusCode execute();
      StatusCode finalize();
  
      void push_back( IAlgorithm *alg );
  
    private:
  
      void dump( Event & );
  
      /*! Clear the ntuple */
      void clear();
  
      TTree *m_tree;
      
      std::vector<IAlgorithm*> m_algs;
      SG::StoreGate *m_store;
  
      std::string m_outputFile;
      int m_outputLevel;
      float m_nEvent;

    
  
      /*! Ntuple output */
      int m_runNumber;
      int m_eventNumber;
      float m_avg_mu;
      std::vector<int>   *m_p_isMain    ;
      std::vector<int>   *m_p_pdg_id    ; 
      std::vector<int>   *m_p_bc_id     ; 
      std::vector<int>   *m_p_seed_id   ; 
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
}

#endif

