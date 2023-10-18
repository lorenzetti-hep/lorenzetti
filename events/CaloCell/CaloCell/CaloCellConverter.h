#ifndef CaloCellConverter_h
#define CaloCellConverter_h

/** simulator includes **/
#include "CaloCell/CaloCell.h"

namespace xAOD{


    typedef std::map<const xAOD::CaloCell*, int> cell_links_t;

    struct CaloCell_t{
        float e;
        float et;
        float tau; 
        float eta;
        float phi;
        float deta;
        float dphi;
        int descriptor_link;
    };

    class CaloCellConverter{

        public:
            CaloCellConverter()=default;
            ~CaloCellConverter()=default;

            // convert a class object into a struct
            bool convert(const CaloCell *, CaloCell_t & , int link);

            bool convert( const CaloCell_t &, CaloCell *& );

        private:

    };

}
#endif


