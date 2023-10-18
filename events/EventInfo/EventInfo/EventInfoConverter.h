#ifndef EventInfoConverter_h
#define EventInfoConverter_h

/** simulator includes **/
#include "EventInfo/EventInfoContainer.h"

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

            // convert a class object into a struct
            bool convert(const EventInfo *truth, EventInfo_t &truth_t ) const;
            bool convert(const EventInfo_t & , EventInfo *&) const;

    };
}
#endif


