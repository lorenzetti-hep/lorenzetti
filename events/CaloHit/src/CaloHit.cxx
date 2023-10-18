
#include "CaloHit/CaloHit.h"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
using namespace xAOD;



CaloHit::CaloHit(     float eta, 
                      float phi, 
                      float deta, 
                      float dphi,
                      // Hash
                      unsigned long int hash,
                      // cell identification
                      CaloSampling sampling, 
                      Detector detector,
                      // bunch crossing information
                      float bc_duration, 
                      int bcid_start, 
                      int bcid_end 
                  ):
  EDM(),
  m_sampling(sampling),
  m_detector(detector),
  m_eta(eta),
  m_phi(phi),
  m_deta(deta),
  m_dphi(dphi),
  m_bcid_start(bcid_start),
  m_bcid_end(bcid_end),
  m_bc_duration(bc_duration),
  m_hash(hash),
  m_firstHit(false)
{
  // Initalize the time vector using the bunch crossing informations
  float start = ( m_bcid_start - 0.5 ) * m_bc_duration;
  float step  = m_bc_duration;
  int total   = (m_bcid_end - m_bcid_start+1) + 1;
  for (int t = 0; t < total; ++t) {
    m_time.push_back( (start + step*t) );
  }
}

void CaloHit::clear()
{
  m_edep.clear(); // zeroize deposit energy for all bunchs
}



void CaloHit::fill( const G4Step* step )
{
  // Get total energy deposit
  float edep = (float)step->GetTotalEnergyDeposit();
  G4StepPoint* point = step->GetPreStepPoint();
  // Get the particle time
  float t = (float)point->GetGlobalTime() / ns;

  // Get the bin index into the time vector
  int samp = find(t);
  if ( samp != -1 ){
    int bcid = m_bcid_start + samp;

    // cout << "sampling: "<< m_sampling << ", BCID: "<< bcid << ", samp: " << samp << ", HIT: " << m_hash << ", t: " << t << ", edep: "<< edep << ", m_edep[bcid]="<< m_edep[bcid] << ", m_tof[bcid]="<< m_tof[bcid] << "\n"; // 

    m_edep[bcid]  +=  (edep/MeV);
    m_tof[bcid]   =   t; // the TOF comes from the last hit

  }
}

void CaloHit::fill( const G4Step* step , float sampNoiseStd)
{
  // Get total energy deposit
  float edep = (float)step->GetTotalEnergyDeposit();
  G4StepPoint* point = step->GetPreStepPoint();
  // Get the particle time
  float t = (float)point->GetGlobalTime() / ns;

  // Get the bin index into the time vector
  int samp = find(t);
  if ( samp != -1 ){
    int bcid = m_bcid_start + samp;

    // cout << "sampling: "<< m_sampling << ", BCID: "<< bcid << ", samp: " << samp << ", HIT: " << m_hash << ", t: " << t << ", edep: "<< edep << ", m_edep[bcid]="<< m_edep[bcid] << ", m_tof[bcid]="<< m_tof[bcid] <<", m_firstHit=" << m_firstHit << "\n"; // 

    m_edep[bcid]+=(edep/MeV);

    if ((m_edep[bcid] > sampNoiseStd/MeV) && !m_firstHit){
      m_tof[bcid] = t; // the TOF comes from the FIRST sensible hit that allows to readout the cell energy, above n*sigmaNoise (n=1)
      m_firstHit  = true;
      // cout << "energy higher than "<< sampNoiseStd <<" MeV: tof="<< t<<"\n";
    }
    else if ((m_edep[bcid] <= sampNoiseStd/MeV) && !m_firstHit){
      m_tof[bcid] = 0.0;
    }
    // else if (m_firstHit){
    //   cout << "first hit TOF already saved. tof="<< m_tof[bcid] <<"\n";
    // }
  }
}




int CaloHit::find( float value) const 
{
  auto binIterator = std::adjacent_find( m_time.begin(), m_time.end(), [=](float left, float right){ return left < value and value <= right; }  );
  if ( binIterator == m_time.end() ) return -1;
  return  binIterator - m_time.begin();
}