#ifndef SeedConverter_h
#define SeedConverter_h

/** simulator includes **/
#include "Seed/Seed.h"

namespace xAOD{

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
            bool convert(const Seed *seed, Seed_t &seed_t );
            bool convert(const Seed_t & , Seed *&);
        private:

    };


}


#endif


