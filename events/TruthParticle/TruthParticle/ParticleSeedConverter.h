#ifndef ParticleSeedConverter_h
#define ParticleSeedConverter_h

/** simulator includes **/
#include "TruthParticle/ParticleSeed.h"

namespace xAOD{

    struct ParticleSeed_t{
        // int pdgid;
        float etot;
        float ettot;
        float eta;
        float phi;
        // float px;
        // float py;
        // float pz;
        // float vx; // vertex position x (prod_vx)
        // float vy; // vertex position y
        // float vz; // vertex position z
        
        // generator seed properties
        // float seedEta;
        // float seedPhi;
        // float seedEtot;
        // float seedEttot;
    };

 
    class ParticleSeedConverter{

        public:
            ParticleSeedConverter()=default;
            ~ParticleSeedConverter()=default;

            // convert a class object into a struct
            bool convert(const ParticleSeed *truth, ParticleSeed_t &truth_t );
            bool convert(const ParticleSeed_t & , ParticleSeed *&);
        private:

    };


}


#endif


