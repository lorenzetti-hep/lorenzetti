#ifndef GenKernel_exceptions_h
#define GenKernel_exceptions_h

#include <exception>

namespace generator{

  class NotInterestingEvent: public std::exception
  {
    virtual const char* what() const throw()
    {
      return "Not interesting event";
    }
  };
  
  
  class AbortPrematurely: public std::exception
  {
    virtual const char* what() const throw()
    {
      return "Abort prematurely";
    }
  };

  class EndOfFile: public std::exception
  {
    virtual const char* what() const throw()
    {
      return "No more events inside";
    }
  };


}
#endif
