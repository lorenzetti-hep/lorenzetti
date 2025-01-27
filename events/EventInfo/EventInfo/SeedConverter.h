#ifndef SeedConverter_h
#define SeedConverter_h

/** simulator includes **/
#include "EventInfo/SeedContainer.h"



namespace xAOD{
    typedef std::map<const xAOD::Seed*, int> seed_links_t;

    struct Seed_t{
        int id;
        float e;
        float et;
        float eta;
        float phi;
    };


 
    class SeedConverter{

        public:
            SeedConverter()=default;
            ~SeedConverter()=default;

            // convert a class object into a struct
            bool convert(const Seed *truth, Seed_t &truth_t ) const;
            bool convert(const Seed_t & , Seed *&) const;
          
    };

}


#endif


