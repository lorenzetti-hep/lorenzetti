#ifndef PythiaGenerator_FixedRegion_h
#define PythiaGenerator_FixedRegion_h

#include "P8Kernel/Algorithm.h"

namespace generator{

  class FixedRegion : public Algorithm
  {
    public:
  
      FixedRegion();
      ~FixedRegion();
  
      virtual StatusCode initialize() override;
      virtual StatusCode execute( generator::Event& ) override;
      virtual StatusCode finalize() override;
  
    private:
  
      float m_eta;
      float m_phi;
      
  };
}
#endif
