#ifndef "IAlgTool_h"
#define "IAlgTool_h"

/** simulator libs **/
#include "IAlgTool.h"
#include "edm/EventContext.h"
#include "tools/MonGroup.h"

/** standard libs **/
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
    virtual setGroup( Monitored::MonGroup *monGroup ) override;
  
  protected:
    /** get the event context **/
    EventContext* getContext();
    /** get the monitoring tool **/
    Monitored::MonGroup* group();

  private:
    // the tool name
    std::string name;
    // the event context
    EventContext *m_context;
    // the monitoring tool
    Monitored::MonGroup *m_monGroup;

};


#endif


