#ifndef PythiaGenerator_Zee_h
#define PythiaGenerator_Zee_h

#include "P8Kernel/Algorithm.h"


namespace generator{

  class Zee : public Algorithm
  {
    public:
      Zee();
      ~Zee();
  
      virtual StatusCode initialize() override;
      virtual StatusCode execute( generator::Event& ) override;
      virtual StatusCode finalize() override;
  
    private:
  
      float m_minPt;
      float m_etaMax;
      std::string m_file;
     
  };

}
#endif
