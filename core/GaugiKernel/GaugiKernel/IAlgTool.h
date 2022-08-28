#ifndef IAlgTool_h
#define IAlgTool_h

#include "GaugiKernel/DataHandle.h"
#include "GaugiKernel/StoreGate.h"
#include "GaugiKernel/StatusCode.h"
#include "GaugiKernel/Property.h"
#include "GaugiKernel/MsgStream.h"
#include "GaugiKernel/EDM.h"


// Declare forward
namespace xAOD{
  class EventInfo;
}


namespace Gaugi{

class IAlgTool
{
	public:
	    
	/*! Destructor */
	virtual ~IAlgTool() {};

	/*! Create all resouces here */    
	virtual StatusCode initialize()=0;
	virtual StatusCode execute( SG::EventContext &ctx, Gaugi::EDM * ) const=0;
	/*! Destroy all allocated memory and close all services */
	virtual StatusCode finalize()=0;

};
}// namespace
#endif
