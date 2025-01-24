#ifndef ElectronConverter_h
#define ElectronConverter_h

/** simulator includes **/
#include "Particle/Electron.h"
#include "CaloCluster/CaloClusterConverter.h"

namespace xAOD{

    struct Electron_t{
        int cluster_link;
        float e;
        float et;
        float eta;
        float phi;
        std::vector<bool> isEM;
    };

    class ElectronConverter{

        public:
            ElectronConverter()=default;
            ~ElectronConverter()=default;

            // convert a class object into a struct
            bool convert( const Electron *, Electron_t & , cluster_links_t &) ;
            bool convert( const Electron_t &electron_t, Electron *&electron );

        private:

    };



}
#endif


