#include "CaloCell/CaloDetDescriptorConverter.h"
//#include "G4Kernel/macros.h"


using namespace xAOD;



bool CaloDetDescriptorConverter::convert( const CaloDetDescriptor *descriptor, CaloDetDescriptor_t &descriptor_t, int link )
{

  if(descriptor){
    descriptor_t.sampling    = (int)descriptor->sampling();
    descriptor_t.detector    = (int)descriptor->sampling();
    descriptor_t.eta         = descriptor->eta();
    descriptor_t.phi         = descriptor->phi();
    descriptor_t.deta        = descriptor->deltaEta(); 
    descriptor_t.dphi        = descriptor->deltaPhi();
    descriptor_t.rmin        = descriptor->rmin(); 
    descriptor_t.rmax        = descriptor->rmax();
    descriptor_t.e           = descriptor->e();

    descriptor_t.bcid_start  = descriptor->bcid_start();
    descriptor_t.bcid_end    = descriptor->bcid_end();
    descriptor_t.bcid_truth  = descriptor->bcid_truth();
    descriptor_t.bc_duration = descriptor->bc_duration();

    descriptor_t.hash        = descriptor->hash();
    descriptor_t.cell_link   = link; // cross link to cell
    return true;
  }

  return false;
}