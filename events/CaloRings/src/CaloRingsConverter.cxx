
#include "CaloRings/CaloRingsConverter.h"
//#include "G4Kernel/macros.h"

#include<iostream>
using namespace xAOD;
using namespace std;


bool CaloRingsConverter::convert( const CaloRings *rings, CaloRings_t &rings_t, cluster_links_t &cluster_links )
{
  if(rings){
    
    rings_t.cluster_link = cluster_links[rings->caloCluster()];
    rings_t.rings = rings->rings();
    return true;
  }
  return false;
}

// bool CaloRingsConverter::convert( const CaloRings_t &rings_t, CaloRings *&rings, CaloCluster, CaloCluster *clus )
bool CaloRingsConverter::convert( const CaloRings_t &rings_t, CaloRings *&rings)
{
  
    rings = new xAOD::CaloRings();
    rings->setRings(rings_t.rings);
    // std::cout << rings_t.cluster_links[0]->eta() << std::endl;
    return true;

  return false;
}