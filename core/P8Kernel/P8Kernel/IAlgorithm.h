
#ifndef P8Kernel_IAlgorithm_h
#define P8Kernel_IAlgorithm_h


#include "GaugiKernel/StoreGate.h"
#include "GaugiKernel/StatusCode.h"
#include "GaugiKernel/Property.h"
#include "GaugiKernel/MsgStream.h"
#include "P8Kernel/Event.h"

namespace generator{

  class IAlgorithm
  {
    public:
      /*! constructor */
      virtual ~IAlgorithm(){};
      /*! Initialize the algorithm */
      virtual StatusCode initialize()=0;
      /*! execute the event selection */
      virtual StatusCode execute( Event &  )=0;
      /*! finalize the algorithm */
      virtual StatusCode finalize()=0;
  };

}
#endif
