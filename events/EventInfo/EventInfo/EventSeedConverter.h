#ifndef EventSeedConverter_h
#define EventSeedConverter_h

/** simulator includes **/
#include "EventInfo/EventSeedContainer.h"



namespace xAOD{

    struct EventSeed_t{
        int id;
        float e;
        float et;
        float eta;
        float phi;
    };

 
    class EventSeedConverter{

        public:
            EventSeedConverter()=default;
            ~EventSeedConverter()=default;

            // convert a class object into a struct
            bool convert(const EventSeed *truth, EventSeed_t &truth_t ) const;
            bool convert(const EventSeed_t & , EventSeed *&) const;
          
    };

}


#endif


