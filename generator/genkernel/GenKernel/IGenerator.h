#ifndef GenKernel_IGenerator_h
#define GenKernel_IGenerator_h

#include "GaugiKernel/StatusCode.h"
#include "GaugiKernel/StoreGate.h"
#include "GaugiKernel/Property.h"
#include "GaugiKernel/MsgStream.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/Units.h"


namespace generator{


  class IGenerator: public MsgService,
                    public Gaugi::PropertyService
  {
    public:
      /*! Constructor */
      IGenerator():
        PropertyService()
      {;}
      
      /*! Destructor */
      virtual ~IGenerator(){};
      
      virtual StatusCode initialize()=0;
      virtual StatusCode execute( HepMC3::GenEvent &)=0;
      virtual StatusCode finalize()=0;

      virtual float random_flat()=0;
      virtual float random_gauss()=0;
      virtual void clear()=0;

      const std::string& name() const;

  };

}
#endif
