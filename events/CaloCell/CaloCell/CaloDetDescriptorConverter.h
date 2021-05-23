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
        float etruth;
        int bcid_start;
        int bcid_end;
        int bcid_truth;
        float bc_duration;
        //std::vector<float> pulse;
        //std::vector<float> time;
        //std::map< int, float> edep;
        //std::map< int, std::vector<float> > pulsePerBunch;
        unsigned int hash;
        int cell_link;
    };

    class CaloDetDescriptorConverter{

        public:
            CaloDetDescriptorConverter()=default;
            ~CaloDetDescriptorConverter()=default;

            // convert a class object into a struct
            bool convert(const CaloDetDescriptor *, CaloDetDescriptor_t &, int link );

        private:

    };

}
#endif