#ifndef RootStreamConverter_h
#define RootStreamConverter_h


#include "GaugiKernel/IRootStreamConverter.h"


class RootStreamConverter    : public IRootStreamConverter, 
                               public MsgService, 
                               public PropertyService
{
  public:

     /*! Constructor */
    RootStreamConverter();
    virtual ~RootStreamConverter() {};

  protected:
    
};
#endif
