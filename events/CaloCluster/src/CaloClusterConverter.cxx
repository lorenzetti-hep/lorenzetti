
#include "CaloCluster/CaloClusterConverter.h"
//#include "G4Kernel/macros.h"


using namespace xAOD;



bool CaloClusterConverter::convert( const CaloCluster *clus, CaloCluster_t &clus_t , cell_links_t &cell_links )
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

      
      for(auto &cell : clus->cells())
      {
        clus_t.cell_links.push_back( cell_links[cell] );
      }
      

      return true;
  }
  return false;
}