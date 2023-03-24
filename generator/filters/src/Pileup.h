#ifndef Evtgen_Pileup_h
#define Evtgen_Pileup_h

#include "GenKernel/IAlgorithm.h"
#include "GenKernel/IGenerator.h"

namespace generator{

  class Pileup: public IAlgorithm 
  {
    public:
  
      Pileup(const std::string , IGenerator *);
      ~Pileup();
  
      virtual StatusCode initialize() override;
      virtual StatusCode execute( Event& ) override;
      virtual StatusCode finalize() override;
  
    private:
      
      int m_select;
      int m_bc_id_start;
      int m_bc_id_end;
      float m_etaMax;
      float m_nPileupAvg;
      float m_nPileupSigma;
      float m_delta_eta;
      float m_delta_phi;

  };
}
#endif
