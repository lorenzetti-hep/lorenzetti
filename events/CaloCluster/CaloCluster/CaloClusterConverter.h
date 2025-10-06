#ifndef CaloClusterConverter_h
#define CaloClusterConverter_h
/** simulator includes **/
#include "CaloCluster/CaloCluster.h"
#include "CaloCell/CaloCellConverter.h"
#include "EventInfo/SeedConverter.h"
#include <map>
namespace xAOD{
    typedef std::map<int, const xAOD::CaloCluster*> cluster_links_t;

    struct CaloCluster_t{
        float e;
        float et;
        float eta;
        float phi;
        float deta;
        float dphi;
        float e0;
        float e1;
        float e2;
        float e3;
        float ehad1;
        float ehad2;
        float ehad3;
        float etot;
        float e233;
        float e237;
        float e277;
        float emaxs1;
        float emaxs2;
        float e2tsts1;
        float reta;
        float rphi;
        float rhad;
        float rhad1;
        float eratio;
        float f0;
        float f1;
        float f2;
        float f3;
        float weta2;
        float secondR;
  	  	float lambdaCenter;
  	  	float secondLambda;
  	  	float fracMax;
  	 	float lateralMom;
  	  	float longitudinalMom;
        std::vector<unsigned long int> cell_links;
        int seed_link;
    };

    class CaloClusterConverter{
        public:
            CaloClusterConverter()=default;
            ~CaloClusterConverter()=default;
            // convert a class object into a struct
            bool convert(const CaloCluster *, CaloCluster_t & );
            bool convert( const CaloCluster_t &, CaloCluster *& );
        private:
    };
}
#endif