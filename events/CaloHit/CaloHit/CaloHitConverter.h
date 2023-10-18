#ifndef CaloHitConverter_h
#define CaloHitConverter_h

/** simulator includes **/
#include "CaloHit/CaloHit.h"

namespace xAOD{

    typedef std::map<const CaloHit *, int> descriptor_links_t;


    struct CaloHit_t{

        int sampling;
        int detector;
        float eta;
        float phi;
        float deta;
        float dphi;
        int bcid_start;
        int bcid_end;
        float bc_duration;
        std::vector<float> edep;
        std::vector<float> tof;
        unsigned long int hash;
    };

    class CaloHitConverter{

        public:
            CaloHitConverter()=default;
            ~CaloHitConverter()=default;

            // convert a class object into a struct
            bool convert(const CaloHit *, CaloHit_t & );
            bool convert( const CaloHit_t &, CaloHit *& );
            

    };

}
#endif