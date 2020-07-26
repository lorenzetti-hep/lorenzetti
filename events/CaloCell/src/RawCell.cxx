
#include "CaloCell/RawCell.h"
#include "CaloCell/enumeration.h"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

using namespace xAOD;
using namespace CaloSampling;


RawCell::RawCell( float eta, 
                  float phi, 
                  float deta, 
                  float dphi, 
                  float radius_min, 
                  float radius_max,
                  std::string hash,
                  int channel_eta, 
                  int channel_phi, 
                  CaloSample sampling, 
                  float bc_duration,
                  int bc_nsamples,
                  int bcid_start,
                  int bcid_end,
                  int bcid_truth ):
  m_sampling(sampling),
  m_channel_eta(channel_eta),
  m_channel_phi(channel_phi),
  m_eta(eta),
  m_phi(phi),
  m_deta(deta),
  m_dphi(dphi),
  m_radius_min(radius_min),
  m_radius_max(radius_max),
  m_rawEnergy(0),
  m_truthRawEnergy(0),
  /* Bunch crossing information */
  m_bcid_start( bcid_start ),
  m_bcid_end( bcid_end ),
  m_bc_nsamples( bc_nsamples ),
  m_bcid_truth( bcid_truth ),
  m_bc_duration( bc_duration ),
  m_rawEnergySamples( (bcid_end-bcid_start+1)*bc_nsamples, 0 ),
  m_hash(hash)
{
  // Initalize the time vector using the bunch crossing informations
  float start = ( m_bcid_start - 0.5 ) * m_bc_duration;
  float step  = m_bc_duration / m_bc_nsamples;
  int total   = (m_bcid_end - m_bcid_start+1) * m_bc_nsamples + 1;
  for (int t = 0; t < total; ++t) {
    m_time.push_back( (start + step*t) );
  }
}


void RawCell::clear()
{
  m_rawEnergy=0.0;
  m_truthRawEnergy=0.0;
  for (std::vector<float>::iterator it = m_rawEnergySamples.begin(); it < m_rawEnergySamples.end(); it++)
  {
    *it=0.0;
  }
}


void RawCell::Fill( const G4Step* step )
{
  // Get total energy deposit
  float edep = (float)step->GetTotalEnergyDeposit();
  G4StepPoint* point = step->GetPreStepPoint();
  // Get the position
  G4ThreeVector pos = point->GetPosition();
  // Get the particle time
  float t = (float)point->GetGlobalTime() / ns;
  
  // Loop over all samples
  for(unsigned int sample=0; sample < m_rawEnergySamples.size(); ++sample){
    if( t >= m_time[sample] && t < m_time[sample+1]){
      m_rawEnergySamples[sample]+=(edep/MeV);
      break;
    }
  }
  if ( t >= ( (m_bcid_truth-1)*m_bc_duration) && t < ((m_bcid_truth+1)*m_bc_duration)){
    m_truthRawEnergy+=(edep/MeV);
  }
}


