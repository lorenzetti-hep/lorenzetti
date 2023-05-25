#ifndef SpacePointConverter_h
#define SpacePointConverter_h

/** simulator includes **/
#include "SpacePoint/SpacePoint.h"

namespace xAOD{

    typedef std::map<const SpacePoint *, int> descriptor_links_t;


    struct SpacePoint_t{

        int sampling;
        int detector;
        float x;
        float y;
        float z;
        float phi;
        float dz;
        float dphi;
        int bcid_start;
        int bcid_end;
        float bc_duration;
        std::vector<float> edep;
        unsigned long int hash;
    };

    class SpacePointConverter{

        public:
            SpacePointConverter()=default;
            ~SpacePointConverter()=default;

            // convert a class object into a struct
            bool convert(const SpacePoint *, SpacePoint_t & );

            bool convert( const SpacePoint_t &, SpacePoint *& );
            
        private:

    };

}
#endif