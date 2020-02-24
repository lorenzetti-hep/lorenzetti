#ifndef AlgTool_h
#define AlgTool_h

/** simulator includes **/
#include "core/IAlgTool.h"
#include "core/EventContext.h"
#include "core/StoreGate.h"

/** standard includes **/
#include <string>

class AlgTool : public IAlgTool
{
  public:
    /** Constructor **/
    AlgTool( std::string & );
    /** Destructor **/
    virtual ~AlgTool();
    /** return the tool name **/
    virtual std::string name() override;
    /** set the event context **/
    virtual void setContext( EventContext *ctx ) override;
    /** set the monitoring tool **/
    virtual void setStoreGateSvc( SG::StoreGate *store ) override;
  
  protected:
    /** get the event context **/
    EventContext* getContext();
    /** get the monitoring tool **/
    SG::StoreGate* getStoreGateSvc();

  private:
    // the tool name
    std::string m_name;
    // the event context
    EventContext *m_context;
    // the monitoring tool
    SG::StoreGate *m_store;

};


#endif


