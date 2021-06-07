#ifndef CaloDetDescriptorConverter_h
#define CaloDetDescriptorConverter_h

/** simulator includes **/
#include "CaloCell/CaloDetDescriptor.h"

namespace xAOD{

    typedef std::map<const CaloDetDescriptor *, int> descriptor_links_t;


    struct CaloDetDescriptor_t{

        int sampling;
        int detector;
        float eta;
        float phi;
        float deta;
        float dphi;
        float rmin; 
        float rmax;
        float e;
        float edep;
        int bcid_start;
        int bcid_end;
        float bc_duration;
        std::vector<float> pulse;
        std::vector<float> edep_per_bunch;
        unsigned long int hash;
        int cell_link;
    };

    class CaloDetDescriptorConverter{

        public:
            CaloDetDescriptorConverter()=default;
            ~CaloDetDescriptorConverter()=default;

            // convert a class object into a struct
            bool convert(const CaloDetDescriptor *, CaloDetDescriptor_t &, int link );

            bool convert( const CaloDetDescriptor_t &, CaloDetDescriptor *& );
            
        private:

    };

}
#endif