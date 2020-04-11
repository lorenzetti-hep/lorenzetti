#ifndef PrimaryGenerator_h
#define PrimaryGenerator_h

#include "G4VPrimaryGenerator.hh"
#include "GaugiKernel/StatusCode.h"
#include "GaugiKernel/MsgStream.h"
#include "GaugiKernel/Property.h"
#include "GaugiKernel/macros.h"



class PrimaryGenerator : public G4VPrimaryGenerator, 
                         public MsgService, 
                         public Gaugi::PropertyService 
{
   
  public:

    /** Constructor **/
    PrimaryGenerator(): G4VPrimaryGenerator(), Gaugi::PropertyService() {};
    /** Destructor **/
    virtual ~PrimaryGenerator()=default;
    
    virtual StatusCode initialize()=0;
    virtual StatusCode finalize()=0;

    virtual PrimaryGenerator* copy()=0;

};
#endif
