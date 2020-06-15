#ifndef P8Kernel_Algorithm_h
#define P8Kernel_Algorithm_h

#include "GaugiKernel/StatusCode.h"
#include "GaugiKernel/StoreGate.h"
#include "GaugiKernel/Property.h"
#include "GaugiKernel/MsgStream.h"
#include "Pythia8/Pythia.h"
#include "Pythia8/HeavyIons.h"
#include "Pythia8Plugins/HepMC2.h"
#include "P8Kernel/exceptions.h"
#include "P8Kernel/Event.h"
#include "P8Kernel/IAlgorithm.h"

namespace generator{

  class Algorithm  : public IAlgorithm,
                     public MsgService,
                     public Gaugi::PropertyService
  {
    public:
      /*! Constructor */
      Algorithm();
      /*! Destructor */
      virtual ~Algorithm() {};
  
      const std::string& name() const;
  
    protected:
      
      /*! Poisson random number generation*/ 
      int poisson(double nAvg);
      /*! Level message */
      int m_outputLevel;
      /*! Pythia seed */
      int m_seed;
      /* Max number of errors */
      int m_nAbort;
      /*! Number of errors */
      int m_iAbort;
      /*! Used to random t for the current processs */
      float m_sigma_t;
      /*! Used to random z for the current processs */
      float m_sigma_z;

      /*! The pythia event generator */
      Pythia8::Pythia m_generator;
  };

}
#endif
