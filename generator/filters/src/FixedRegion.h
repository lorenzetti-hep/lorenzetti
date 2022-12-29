#ifndef Evtgen_FixedRegion_h
#define Evtgen_FixedRegion_h

#include "GenKernel/IAlgorithm.h"

namespace generator{

  class FixedRegion : public IAlgorithm
  {
    public:
  
      FixedRegion(const std::string name);
      ~FixedRegion()=default;
  
      virtual StatusCode initialize() override;
      virtual StatusCode execute( Event& ) override;
      virtual StatusCode finalize() override;
  
    private:
  
      float m_eta;
      float m_phi;
      
  };
}
#endif
