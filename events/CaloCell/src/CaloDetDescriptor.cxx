
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
                  float radius_min, 
                  float radius_max,
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
  m_radius_min(radius_min),
  m_radius_max(radius_max),
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

