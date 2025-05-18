#include "CaloCell/CaloDetDescriptorConverter.h"
//#include "G4Kernel/macros.h"


using namespace xAOD;



bool CaloDetDescriptorConverter::convert( const CaloDetDescriptor *descriptor, CaloDetDescriptor_t &descriptor_t )
{

  if(descriptor){
    descriptor_t.sampling    = (int)descriptor->sampling();
    descriptor_t.detector    = (int)descriptor->detector();
    descriptor_t.eta         = descriptor->eta();
    descriptor_t.phi         = descriptor->phi();
    descriptor_t.deta        = descriptor->deltaEta(); 
    descriptor_t.dphi        = descriptor->deltaPhi();
    descriptor_t.e           = descriptor->e();
    descriptor_t.edep        = descriptor->edep(); // truth energy into the bunch crossing zero
    descriptor_t.tau         = descriptor->tau();
    descriptor_t.bcid_start  = descriptor->bcid_start();
    descriptor_t.bcid_end    = descriptor->bcid_end();
    descriptor_t.bc_duration = descriptor->bc_duration();
    descriptor_t.hash        = descriptor->hash();
    descriptor_t.pulse       = descriptor->pulse();
    descriptor_t.z           = descriptor->z();
    descriptor_t.anomalous   = descriptor->anomalous();


    for ( int bcid = descriptor->bcid_start();  bcid <= descriptor->bcid_end(); ++bcid )
    {
      descriptor_t.edep_per_bunch.push_back( descriptor->edep(bcid) );
      descriptor_t.tof.push_back ( descriptor->tof(bcid)  ); //
    }

    return true;
  }

  return false;
}



bool CaloDetDescriptorConverter::convert( const CaloDetDescriptor_t &descriptor_t, CaloDetDescriptor *&descriptor )
{

  descriptor = new xAOD::CaloDetDescriptor( descriptor_t.eta, 
                                            descriptor_t.phi,
                                            descriptor_t.deta,
                                            descriptor_t.dphi,
                                            descriptor_t.hash,
                                            descriptor_t.z,
                                            (CaloSampling)descriptor_t.sampling, 
                                            (Detector)descriptor_t.detector,
                                            descriptor_t.bc_duration,
                                            descriptor_t.bcid_start,
                                            descriptor_t.bcid_end);

  descriptor->setE(descriptor_t.e); // estimated energy from OF
  descriptor->setPulse( descriptor_t.pulse); // pulse from generator
  descriptor->setTau( descriptor_t.tau );
  for ( int bcid = descriptor->bcid_start();  bcid <= descriptor->bcid_end(); ++bcid )
  {
    descriptor->edep( bcid, descriptor_t.edep_per_bunch[bcid] ); // truth energy for each bunch crossing
    descriptor->tof ( bcid, descriptor_t.tof[bcid]  ); // truth time of flight (it takes the last hit in the simulation order. Need to evaluate which strategy is the best.)
  }
  return true;
}
