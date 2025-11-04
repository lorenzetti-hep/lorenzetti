#ifndef CaloDetDescriptorConverter_h
#define CaloDetDescriptorConverter_h

/** simulator includes **/
#include "CaloCell/CaloDetDescriptor.h"

namespace xAOD{

    typedef std::map<unsigned long int, const CaloDetDescriptor *> descriptor_links_t;


    struct CaloDetDescriptor_t{

        int sampling;
        int detector;
        float eta;
        float phi;
        float deta;
        float dphi;
        float e;
        float tau;
        float edep;
        int bcid_start;
        int bcid_end;
        float bc_duration;
        std::vector<float> pulse;
        std::vector<float> edep_per_bunch;
        std::vector<float> tof;
        unsigned long int hash;
        float z;
        bool anomalous;
        float sigma;
    };

    class CaloDetDescriptorConverter{

        public:
            CaloDetDescriptorConverter()=default;
            ~CaloDetDescriptorConverter()=default;

            // convert a class object into a struct
            bool convert(const CaloDetDescriptor *, CaloDetDescriptor_t &);

            bool convert( const CaloDetDescriptor_t &, CaloDetDescriptor *& );
            
        private:

    };

}
#endif