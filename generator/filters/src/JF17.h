#ifndef Evtgen_JF17_h
#define Evtgen_JF17_h

#include "GenKernel/IAlgorithm.h"
#include "GenKernel/IGenerator.h"

namespace generator{

  class JF17 : public IAlgorithm
  {
    public:
  
      JF17(const std::string , IGenerator *);
      ~JF17();
  
      virtual StatusCode initialize() override;
      virtual StatusCode execute( generator::Event& ) override;
      virtual StatusCode finalize() override;
  
    private:
  
      int m_select;
      float m_minPt;
      float m_etaMax;
      float m_etaMin;
      float m_etaWindow;
      float m_phiWindow;
      
  };
}
#endif
