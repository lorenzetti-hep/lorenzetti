#ifndef CaloRingsConverter_h
#define CaloRingsConverter_h

/** simulator includes **/
#include "CaloRings/CaloRings.h"
#include "CaloCluster/CaloClusterConverter.h"

namespace xAOD{

    struct CaloRings_t{
        int cluster_link;
        std::vector<float> rings;
    };

    class CaloRingsConverter{

        public:
            CaloRingsConverter()=default;
            ~CaloRingsConverter()=default;

            // convert a class object into a struct
            bool convert( const CaloRings *, CaloRings_t & , cluster_links_t &) ;
            bool convert( const CaloRings_t &rings_t, CaloRings *&rings );

        private:

    };



}
#endif


