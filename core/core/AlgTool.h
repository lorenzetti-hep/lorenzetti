#ifndef IAlgTool_h
#define IAlgTool_h

/** simulator includes **/
#include "IAlgTool.h"
#include "EventContext.h"
#include "StoreGate.h"

/** standard includes **/
#include <string>

class AlgTool : public IAlgTool
{
  public:
    /** Constructor **/
    virtual AlgTool( std::string & );
    /** Destructor **/
    virtual ~AlgTool();
    /** return the tool name **/
    virtual std::string name() override;
    /** set the event context **/
    virtual setContext( EventContext *ctx ) override;
    /** set the monitoring tool **/
    virtual setStoreGateSvc( SG::StoreGate *store ) override;
  
  protected:
    /** get the event context **/
    EventContext* getContext();
    /** get the monitoring tool **/
    SG::StoreGate* getStoreGateSvc();

  private:
    // the tool name
    std::string name;
    // the event context
    EventContext *m_context;
    // the monitoring tool
    SG::StoreGate *m_store;

};


#endif


