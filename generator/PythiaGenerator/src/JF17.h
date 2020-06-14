#ifndef PythiaGenerator_JF17_h
#define PythiaGenerator_JF17_h

#include "P8Kernel/Algorithm.h"

namespace generator{

  class JF17 : public Algorithm
  {
    public:
  
      JF17();
      ~JF17();
  
      virtual StatusCode initialize() override;
      virtual StatusCode execute( generator::Event& ) override;
      virtual StatusCode finalize() override;
  
    private:
  
      int m_select;
      float m_minPt;
      float m_etaMax;
      float m_etaWindow;
      float m_phiWindow;
      std::string m_file;
      
  };
}
#endif
