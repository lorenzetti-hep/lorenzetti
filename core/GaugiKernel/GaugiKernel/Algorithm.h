#ifndef Algorithm_h
#define Algorithm_h


#include "GaugiKernel/IAlgorithm.h"
#include "GaugiKernel/DataHandle.h"
#include "GaugiKernel/StoreGate.h"
#include "GaugiKernel/StatusCode.h"
#include "GaugiKernel/Property.h"
#include "GaugiKernel/MsgStream.h"



namespace Gaugi{


  class Algorithm  : public IAlgorithm, 
                     public MsgService, 
                     public PropertyService
  {
    public:

      /*! Constructor */
      Algorithm( std::string &name, MSG::Level level );


      virtual ~Algorithm() {};

      std::string name();

      /*! set the store gate service */
      void setStoreGateSvc( SG::StoreGate * );
    
    protected:
      
      /** get the monitoring tool **/
      SG::StoreGate* getStoreGateSvc();
  
    private:
      
      std::string m_name;

      // the monitoring tool
      SG::StoreGate *m_store;
  
  };



}
#endif
