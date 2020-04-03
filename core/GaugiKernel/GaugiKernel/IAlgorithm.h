#ifndef IAlgService_h
#define IAlgService_h

#include "GaugiKernel/DataHandle.h"
#include "GaugiKernel/StoreGate.h"
#include "GaugiKernel/StatusCode.h"
#include "GaugiKernel/Property.h"
#include "GaugiKernel/MsgStream.h"




namespace Gaugi{


	/* Algortihm base service for all recontruction algorithms */
	class IAlgorithm
	{
	
	  public:
	    
			/*! Destructor */
	    virtual ~IAlgorithm() {};

			/*! Create all resouces here */    
	    virtual StatusCode initialize()=0;
			
			/*! Destroy all allocated memory and close all services */
			virtual StatusCode finalize()=0;
	
			/*! This step will be executed before Geant step action */
	    virtual StatusCode pre_execute( SG::EventContext & /*ctx*/ )=0;
			
			/*! This step will be executed during the Geant step action */
			virtual StatusCode execute( SG::EventContext &/*ctx*/ )=0;
			
			/*! This step will be executed after the Geant step action */
	    virtual StatusCode post_execute( SG::EventContext & /*ctx*/ )=0;
	    
			/*! Fill all monitoring histograms here */
	    virtual StatusCode fillHistograms( SG::EventContext & /*ctx*/ )=0;
	


	};

	

}// namespace
#endif
