
#include "Egamma/ElectronConverter.h"
//#include "G4Kernel/macros.h"

#include<iostream>
using namespace xAOD;
using namespace std;


bool ElectronConverter::convert( const Electron *electron, Electron_t &electron_t, cluster_links_t &cluster_links )
{
  if(electron){
    electron_t.eta = electron->eta();
    electron_t.phi = electron->phi();
    electron_t.et  = electron->et();
    electron_t.e   = electron->e();
    electron_t.isEM.push_back(electron->isTight());
    electron_t.isEM.push_back(electron->isMedium());
    electron_t.isEM.push_back(electron->isLoose());
    electron_t.isEM.push_back(electron->isVeryLoose());
    electron_t.cluster_link = cluster_links[electron->caloCluster()];
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
    electron->setIsTight(electron_t.isEM[0]);
    electron->setIsMedium(electron_t.isEM[1]);  
    electron->setIsLoose(electron_t.isEM[2]);
    electron->setIsVeryLoose(electron_t.isEM[3]);
    return true;
}