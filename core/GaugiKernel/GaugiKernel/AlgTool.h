#ifndef AlgTool_h
#define AlgTool_h


#include "GaugiKernel/IAlgTool.h"
#include "GaugiKernel/DataHandle.h"
#include "GaugiKernel/StoreGate.h"
#include "GaugiKernel/StatusCode.h"
#include "GaugiKernel/Property.h"
#include "GaugiKernel/MsgStream.h"



namespace Gaugi{


  class AlgTool    : public IAlgTool, 
                     public MsgService, 
                     public PropertyService
  {
    public:

      /*! Constructor */
      AlgTool();


      virtual ~AlgTool() {};

      const std::string& name() const;


    protected:
      
  
      /*! Output level message */
      int m_outputLevel;

  };



}
#endif
