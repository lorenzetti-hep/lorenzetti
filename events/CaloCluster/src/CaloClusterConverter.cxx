
#include "CaloCluster/CaloClusterConverter.h"
//#include "G4Kernel/macros.h"
using namespace xAOD;
bool CaloClusterConverter::convert( const CaloCluster *clus, CaloCluster_t &clus_t , 
                                    cell_links_t &cell_links, seed_links_t &seed_links )
{
  if(clus){
      clus_t.e           = clus->e();
      clus_t.et          = clus->et();
      clus_t.eta         = clus->eta();
      clus_t.phi         = clus->phi();
      clus_t.deta        = clus->deltaEta();
      clus_t.dphi        = clus->deltaPhi();
      clus_t.e0          = clus->e0();
      clus_t.e1          = clus->e1();
      clus_t.e2          = clus->e2();
      clus_t.e3          = clus->e3();
      clus_t.ehad1       = clus->ehad1();
      clus_t.ehad2       = clus->ehad2();
      clus_t.ehad3       = clus->ehad3();
      clus_t.etot        = clus->etot();
      clus_t.e233        = clus->e233();
      clus_t.e237        = clus->e237();
      clus_t.e277        = clus->e277();
      clus_t.emaxs1      = clus->emaxs1();
      clus_t.emaxs2      = clus->emaxs2();
      clus_t.e2tsts1     = clus->e2tsts1();
      clus_t.reta        = clus->reta();
      clus_t.rphi        = clus->rphi();
      clus_t.rhad        = clus->rhad();
      clus_t.rhad1       = clus->rhad1();
      clus_t.eratio      = clus->eratio();
      clus_t.f0          = clus->f0();
      clus_t.f1          = clus->f1();
      clus_t.f2          = clus->f2();
      clus_t.f3          = clus->f3();
      clus_t.weta2       = clus->weta2();
      clus_t.secondR     = clus->secondR();
	    clus_t.lambdaCenter = clus->lambdaCenter();
	    clus_t.secondLambda = clus->secondLambda();
	    clus_t.fracMax     = clus->fracMax();
	    clus_t.lateralMom  = clus->lateralMom();
	    clus_t.longitudinalMom = clus->longitudinalMom();
      clus_t.seed_link   = seed_links[clus->seed()];

      for(auto &cell : clus->cells())
      {
        clus_t.cell_links.push_back( cell_links[cell] );
      }
      
      return true;
  }
  return false;
}
bool CaloClusterConverter::convert( const CaloCluster_t &cluster_t, CaloCluster *&cluster )
{
  cluster = new xAOD::CaloCluster( cluster_t.emaxs2,
                                  cluster_t.eta, 
                                  cluster_t.phi, 
                                  cluster_t.deta, 
                                  cluster_t.dphi );
  cluster->setE(cluster_t.e); // estimated energy from OF. Same as descriptor->e()
  cluster->setEt(cluster_t.et); // estimated transverse energy from OF
  cluster->setE0(cluster_t.e0);
  cluster->setE1(cluster_t.e1);
  cluster->setE2(cluster_t.e2);
  cluster->setE3(cluster_t.e3);
  cluster->setEhad1(cluster_t.ehad1);
  cluster->setEhad2(cluster_t.ehad2);
  cluster->setEhad3(cluster_t.ehad3);
  cluster->setEtot(cluster_t.etot);
  cluster->setE277(cluster_t.e277);
  cluster->setE237(cluster_t.e237);
  cluster->setE233(cluster_t.e233);
  cluster->setReta(cluster_t.reta);
  cluster->setRphi(cluster_t.rphi);
  cluster->setWeta2(cluster_t.weta2);
  cluster->setEratio(cluster_t.eratio);
  cluster->setEmaxs1(cluster_t.emaxs1);
  cluster->setE2tsts1(cluster_t.e2tsts1);
  cluster->setF0(cluster_t.f0);
  cluster->setF1(cluster_t.f1);
  cluster->setF2(cluster_t.f2);
  cluster->setF3(cluster_t.f3);
  cluster->setRhad(cluster_t.rhad);
  cluster->setRhad1(cluster_t.rhad1);
  cluster->setSecondR(cluster_t.secondR);
  cluster->setLambdaCenter(cluster_t.lambdaCenter);
  cluster->setSecondLambda(cluster_t.secondLambda);
  cluster->setFracMax(cluster_t.fracMax);
  cluster->setLateralMom(cluster_t.lateralMom);
  cluster->setLongitudinalMom(cluster_t.longitudinalMom);    
  return true;
}
