#ifndef EventInfoConverter_h
#define EventInfoConverter_h

/** simulator includes **/
#include "EventInfo/EventInfo.h"

namespace xAOD{

    struct EventInfo_t{
        float eventNumber;
        float avgmu;
    };



    class EventInfoConverter{

        public:
            EventInfoConverter()=default;
            ~EventInfoConverter()=default;

            // convert a class object into a struct
            bool convert(const EventInfo *, EventInfo_t & );
            bool convert(const EventInfo_t &, EventInfo *& );
        private:

    };

}
#endif


