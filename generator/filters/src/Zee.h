#ifndef Evtgen_Zee_h
#define Evtgen_Zee_h

#include "GenKernel/IAlgorithm.h"
#include "GenKernel/IGenerator.h"

namespace generator{

  class Zee : public IAlgorithm
  {
    public:
    
      Zee(const std::string , IGenerator *);
      ~Zee();
  
      virtual StatusCode initialize() override;
      virtual StatusCode execute( Event& ) override;
      virtual StatusCode finalize() override;
  
    private:
  
      float m_minPt;
      float m_etaMax;
      bool  m_zeroVertexParticles;
      bool  m_forceForwardElectron;
     
  };

}
#endif
