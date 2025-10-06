#ifndef CaloCellConverter_h
#define CaloCellConverter_h

/** simulator includes **/
#include "CaloCell/CaloCell.h"

namespace xAOD{


    typedef std::map<unsigned long int, const xAOD::CaloCell*> cell_links_t;

    struct CaloCell_t{
        float e;
        float et;
        float tau; 
        float eta;
        float phi;
        float deta;
        float dphi;
        unsigned long int descriptor_link; // NOTE: lets use hash as link
    };

    class CaloCellConverter{

        public:
            CaloCellConverter()=default;
            ~CaloCellConverter()=default;

            // convert a class object into a struct
            bool convert(const CaloCell *, CaloCell_t & );

            bool convert( const CaloCell_t &, CaloCell *& );

        private:

    };

}
#endif


