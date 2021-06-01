#ifndef TruthParticleConverter_h
#define TruthParticleConverter_h

/** simulator includes **/
#include "TruthParticle/TruthParticle.h"

namespace xAOD{

    struct TruthParticle_t{
        int pdgid;
        float e;
        float et;
        float eta;
        float phi;
        float px;
        float py;
        float pz;
    };

 
    class TruthParticleConverter{

        public:
            TruthParticleConverter()=default;
            ~TruthParticleConverter()=default;

            // convert a class object into a struct
            bool convert(const TruthParticle *truth, TruthParticle_t &truth_t );
            bool convert(const TruthParticle_t & , TruthParticle *&);
        private:

    };


}


#endif


