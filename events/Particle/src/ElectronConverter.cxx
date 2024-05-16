
#include "Particle/ElectronConverter.h"
//#include "G4Kernel/macros.h"

#include<iostream>
using namespace xAOD;
using namespace std;


bool ElectronConverter::convert( const Electron *electron, Electron_t &electron_t, cluster_links_t &cluster_links )
{
  if(electron){
    
    electron_t.cluster_link = cluster_links[electron->caloCluster()];
    electron_t.eta = electron->eta();
    electron_t.phi = electron->phi();
    electron_t.et = electron->et();
    electron_t.e = electron->e();
    for (auto dec : electron->getDecisions() ) electron_t.decisions.push_back(dec);
    return true;
  }
  return false;
}


bool ElectronConverter::convert( const Electron_t &electron_t, Electron *&electron)
{
  
    electron = new xAOD::Electron();
    electron->setEta(electron_t.eta);
    electron->setPhi(electron_t.phi);
    electron->setEt(electron_t.et);
    electron->setE(electron_t.e);
    electron->setDecisions(electron_t.decisions);

    return true;

}