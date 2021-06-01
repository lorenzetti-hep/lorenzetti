#ifndef ComponentReader_h 
#define ComponentReader_h

#include "GaugiKernel/IComponentReader.h"
#include "GaugiKernel/MsgStream.h"
#include "GaugiKernel/Property.h"
#include <string>


namespace Gaugi{

  class ComponentReader : public IComponentReader,
                          public MsgService, 
                          public PropertyService         
  {
    public:

      /** Constructor **/
      ComponentReader();
      
      /** Destructor **/
      virtual ~ComponentReader() {};

      /*! Return the name of the algorithm */
      const std::string& name() const;

    protected:
      /*! Level message */
      int m_outputLevel;
   
  };

}
#endif

