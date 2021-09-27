#ifndef P8Kernel_Algorithm_h
#define P8Kernel_Algorithm_h

#include "GaugiKernel/StatusCode.h"
#include "GaugiKernel/StoreGate.h"
#include "GaugiKernel/Property.h"
#include "GaugiKernel/MsgStream.h"
#include "Pythia8/Pythia.h"
#include "Pythia8/HeavyIons.h"
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

      /*! Sample pp z axis fluctuation */ 
      float sample_z() const;
      /*! Sample pp time fluctuation */ 
      float sample_t() const;
      /*! Poisson random number generation*/ 
      int poisson(double nAvg);
  
      float trunc_centered_gauss(float sigma, float trunc) const;
      
      /*! Level message */
      int m_outputLevel;
      /*! Pythia seed */
      int m_seed;
      /* Max number of errors */
      int m_nAbort;
      /*! Number of errors */
      int m_iAbort;
      /*! Time fluctuations in pp collisions */
      float m_sigma_t;
      /*! Z axis fluctuations in pp collisions */
      float m_sigma_z;
      /*! Truncate large t fluctuations (mm) */
      float m_trunc_t;
      /*! Truncate large z fluctuations (mm) */
      float m_trunc_z;
      /*! Set maximum mu to avoid overflow. */
      float m_trunc_mu = 300;

      /*! The pythia event generator */
      mutable Pythia8::Pythia m_generator;
  };

}
#endif
