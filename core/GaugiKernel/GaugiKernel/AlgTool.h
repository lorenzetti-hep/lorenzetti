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
      AlgTool( std::string &name );


      virtual ~AlgTool() {};

      const std::string& name() const;

      /*! set the store gate service */
      void setStoreGateSvc( SG::StoreGate * );
    

    protected:
      
      /** get the monitoring tool **/
      SG::StoreGate* getStoreGateSvc() const;
  
      /*! Output level message */
      int m_outputLevel;

    private:
      
      // the monitoring tool
      SG::StoreGate *m_store;
  
  };



}
#endif
