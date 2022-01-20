#ifndef P8Kernel_EventGenerator_h
#define P8Kernel_EventGenerator_h


#include "GaugiKernel/MsgStream.h"
#include "GaugiKernel/StoreGate.h"
#include "GaugiKernel/StatusCode.h"
#include "GaugiKernel/Property.h"
#include "P8Kernel/Algorithm.h"
#include "P8Kernel/exceptions.h"
#include "TTree.h"
#include <vector>
#include <string>


namespace generator{

  class EventGenerator: public MsgService, 
                        public Gaugi::PropertyService
  {
    public:
  
      EventGenerator();
      ~EventGenerator();
  
      StatusCode initialize();
      StatusCode execute();
      StatusCode finalize();
  
      void push_back( Algorithm *alg );
  
    private:
  
      void dump( generator::Event & );
  
      /*! Clear the ntuple */
      void clear();
  
  
      std::vector<Algorithm*> m_algs;
  
      TTree *m_tree;
      SG::StoreGate *m_store;
  
      std::string m_outputFile;
      int m_outputLevel;
      float m_nEvent;

      std::vector<int> m_eventNumbers;
    
  
      /*! Ntuple output */
      int m_eventNumber;
      float m_avg_mu;
      std::vector<int>   *m_p_isMain    ;
      //std::vector<int>   *m_bc_id_nhits ;
      std::vector<int>   *m_p_pdg_id    ; 
      std::vector<int>   *m_p_bc_id     ; 
      //std::vector<float> *m_bc_id_mu    ; 
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

