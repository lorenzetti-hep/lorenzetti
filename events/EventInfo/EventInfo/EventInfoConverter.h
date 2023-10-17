#ifndef EventInfoConverter_h
#define EventInfoConverter_h

#include "EventInfo/EventInfoContainer.h"
#include "GaugiKernel/DataHandle.h"


namespace xAOD{

    struct EventInfo_t{
        float runNumber;
        float eventNumber;
        float avgmu;
    };


    class EventInfoConverter{

        public:
            EventInfoConverter()=default;
            ~EventInfoConverter()=default;

            bool convert(const EventInfo *truth, EventInfo_t &truth_t ) const;
            bool convert(const EventInfo_t & , EventInfo *&) const;
            bool serialize(  std::string &, SG::EventContext &/*ctx*/, TTree *) const;
            bool deserialize( std::string &, int &, TTree *, SG::EventContext &/*ctx*/) const;

  
    };
}
#endif


