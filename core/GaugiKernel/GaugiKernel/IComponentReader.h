#ifndef IComponentReader_h 
#define IComponentReader_h


#include "GaugiKernel/StatusCode.h"
#include "GaugiKernel/DataHandle.h"
#include <string>


namespace Gaugi{

  class IComponentReader
  {
    public:

      /** Destructor **/
      virtual ~IComponentReader() {};

      virtual StatusCode initialize()=0;
      
      virtual StatusCode GeneratePrimaryVertex( int evt, SG::EventContext & ) const=0;

      virtual StatusCode finalize()=0;

      virtual int GetEntries() const=0;
   
  };

}
#endif

