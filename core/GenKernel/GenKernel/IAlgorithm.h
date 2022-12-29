#ifndef GenKernel_IAlgorithm_h
#define GenKernel_IAlgorithm_h

#include "GaugiKernel/StatusCode.h"
#include "GaugiKernel/StoreGate.h"
#include "GaugiKernel/Property.h"
#include "GaugiKernel/MsgStream.h"
#include "GenKernel/exceptions.h"
#include "GenKernel/IGenerator.h"
#include "GenKernel/Event.h"


namespace generator{

  class IAlgorithm  : public MsgService,
                      public Gaugi::PropertyService
  {
    public:
      /*! Constructor */
      IAlgorithm(IGenerator *gen);
      
      /*! Destructor */
      virtual ~IAlgorithm() {};

      virtual StatusCode initialize()=0;
      virtual StatusCode execute( Event& )=0;
      virtual StatusCode finalize()=0;


      const std::string& name() const;


    protected:

      IGenerator* generator(){return m_generator;};

      /*! Sample pp z axis fluctuation */ 
      float sample_z() const;
      /*! Sample pp time fluctuation */ 
      float sample_t() const;
      /*! Poisson random number generation*/ 
      int poisson(double nAvg);
  
      float trunc_centered_gauss(float sigma, float trunc) const;
      
      /*! Level message */
      int m_outputLevel;
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

    private:
      /*! The event generator */
      IGenerator *m_generator;

  };

}
#endif
