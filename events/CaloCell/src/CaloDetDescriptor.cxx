
#include "CaloCell/CaloDetDescriptor.h"
#include "CaloCell/enumeration.h"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

using namespace xAOD;



CaloDetDescriptor::CaloDetDescriptor( 
                  float eta, 
                  float phi, 
                  float deta, 
                  float dphi, 
                  unsigned long int hash,
                  CaloSampling sampling, 
                  Detector detector,
                  float bc_duration,
                  int bcid_start,
                  int bcid_end ):
  EDM(),
  m_sampling(sampling),
  m_detector(detector),
  m_eta(eta),
  m_phi(phi),
  m_deta(deta),
  m_dphi(dphi),
  m_e(0),
  /* Bunch crossing information */
  m_bcid_start( bcid_start ),
  m_bcid_end( bcid_end ),
  m_bc_duration( bc_duration ),
  m_hash(hash)
{
  // Initalize the time vector using the bunch crossing informations
  float start = ( m_bcid_start - 0.5 ) * m_bc_duration;
  float step  = m_bc_duration;
  int total   = (m_bcid_end - m_bcid_start+1) + 1;
  for (int t = 0; t < total; ++t) {
    m_time.push_back( (start + step*t) );
  }
}

CaloDetDescriptor * CaloDetDescriptor::copy()
{
  auto det = new CaloDetDescriptor( m_eta, 
                                    m_phi, 
                                    m_deta, 
                                    m_dphi, 
                                    m_hash,
                                    m_sampling, 
                                    m_detector,
                                    m_bc_duration,
                                    m_bcid_start,
                                    m_bcid_end );

  det->setE(e()); // estimated energy from OF
  det->setPulse( pulse() ); // pulse from generator
  det->setTau( tau() );
  for ( int bcid = bcid_start();  bcid <= bcid_end(); ++bcid )
  {
    det->edep( bcid, edep(bcid) ); // truth energy for each bunch crossing
    det->tof ( bcid, tof(bcid)  ); // truth time of flight (it takes the last hit in the simulation order. Need to evaluate which strategy is the best.)
  }
  return det;
}