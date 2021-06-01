
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
                  unsigned int hash,
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


void CaloDetDescriptor::clear()
{
  m_e = 0.0; // zeroize energy
  m_edep.clear(); // zeroize deposit energy for all bunchs
  m_pulsePerBunch.clear(); // zeroize all pulses for all bunchs
  m_pulse.clear(); // zeroize the main pulse
}


void CaloDetDescriptor::Fill( const G4Step* step )
{
  // Get total energy deposit
  float edep = (float)step->GetTotalEnergyDeposit();
  G4StepPoint* point = step->GetPreStepPoint();
  // Get the position
  G4ThreeVector pos = point->GetPosition();
  // Get the particle time
  float t = (float)point->GetGlobalTime() / ns;

  // Get the bin index into the time vector
  int samp = findIndex(t);
  if ( samp != -1 ){
    int bcid = m_bcid_start + samp;
    m_edep[bcid]+=(edep/MeV);
  }


}

int CaloDetDescriptor::findIndex( float value) const 
{
  auto binIterator = std::adjacent_find( m_time.begin(), m_time.end(), [=](float left, float right){ return left < value and value <= right; }  );
  if ( binIterator == m_time.end() ) return -1;
  return  binIterator - m_time.begin();
}


