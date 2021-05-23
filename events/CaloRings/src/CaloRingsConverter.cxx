
#include "CaloRings/CaloRingsConverter.h"
//#include "G4Kernel/macros.h"


using namespace xAOD;



bool CaloRingsConverter::convert( const CaloRings *rings, CaloRings_t &rings_t, cluster_links_t &cluster_links )
{
  if(rings){
    
    rings_t.cluster_link = cluster_links[rings->caloCluster()];
    rings_t.rings = rings->rings();
    return true;
  }
  return false;
}