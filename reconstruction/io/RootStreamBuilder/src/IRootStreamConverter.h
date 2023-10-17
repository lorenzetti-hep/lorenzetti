#ifndef IRootStreamConverter_h
#define IRootStreamConverter_h

#include "GaugiKernel/DataHandle.h"
#include "GaugiKernel/StoreGate.h"
#include "GaugiKernel/StatusCode.h"
#include "GaugiKernel/Property.h"
#include "GaugiKernel/MsgStream.h"


class IRootStreamConverter
{
	public:
	    
	/*! Destructor */
	virtual ~IRootStreamConverter() {};

    virtual StatusCode serialize(  std::string &, SG::EventContext &/*ctx*/, TTree *) const=0;
    virtual StatusCode deserialize( std::string &, int &, TTree *, SG::EventContext &/*ctx*/) const=0;


};
#endif
