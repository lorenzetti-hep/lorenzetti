#ifndef PythiaGenerator_Pileup_h
#define PythiaGenerator_Pileup_h

#include "P8Kernel/Algorithm.h"
#include "P8Kernel/Event.h"

namespace generator{

  class Pileup: public Algorithm 
  {
    public:
  
      Pileup();
      ~Pileup();
  
      virtual StatusCode initialize() override;
      virtual StatusCode execute( generator::Event& ) override;
      virtual StatusCode finalize() override;
  
  
  
    private:
      
      int m_select;
      int m_bc_id_start;
      int m_bc_id_end;
      float m_etaMax;
      float m_nPileupAvg;
      float m_delta_eta;
      float m_delta_phi;
      std::string m_file;

    
  };
}
#endif










