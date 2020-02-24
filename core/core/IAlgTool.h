#ifndef IAlgTool_h
#define IAlgTool_h

/** simulator includes **/
#include "core/EventContext.h"
#include "core/StatusCode.h"
#include "core/StoreGate.h"


class IAlgTool{

  public:
    /** Contructor **/
    IAlgTool();
    /** Destructor **/
    virtual ~IAlgTool();

    virtual StatusCode initialize()=0;
    
    /** This will be called before step action **/
    virtual StatusCode pre_execute( EventContext * /*ctx*/ )=0;
    /*
     * Execution in Step action class. This will be used 
     * when geant 4 generate the step point. This method
     * will be call during the step action in Geant 4
     */
    virtual StatusCode execute( EventContext * /*ctx*/ )=0;
    
    /** This will be called after step action **/
    virtual StatusCode post_execute( EventContext * /*ctx*/ )=0;
    
    virtual StatusCode finalize()=0;
    
    virtual StatusCode fill( EventContext * /*ctx*/ )=0;

    /** return the tool name **/
    virtual std::string name()=0;
    /** set the event context **/
    virtual void setContext( EventContext * )=0;
    /** set the monitoring tool **/
    virtual void setStoreGateSvc( SG::StoreGate * )=0;
};
#endif


