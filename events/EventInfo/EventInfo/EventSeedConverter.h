#ifndef EventSeedConverter_h
#define EventSeedConverter_h

/** simulator includes **/
#include "EventInfo/EventSeedContainer.h"
#include "GaugiKernel/DataHandle.h"



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

            bool serialize(  std::string &, SG::EventContext &/*ctx*/, TTree *) const;
            bool deserialize( std::string &, int &, TTree *, SG::EventContext &/*ctx*/) const;
            bool convert(const EventSeed *truth, EventSeed_t &truth_t ) const;
            bool convert(const EventSeed_t & , EventSeed *&) const;

    };

}


#endif


