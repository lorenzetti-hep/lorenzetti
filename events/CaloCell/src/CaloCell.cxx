
#include "CaloCell/CaloCell.h"
#include "CaloCell/enumeration.h"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"



using namespace xAOD;
using namespace CaloSampling;

CaloCell::CaloCell()
{;}


CaloCell::CaloCell( float eta, 
                    float phi, 
                    float deta, 
                    float dphi, 
                    float radius_min, 
                    float radius_max,
                    std::string hash,
                    CaloSample sampling, 
                    float bc_duration
                    int bc_nsamples,
                    int bcid_start,
                    int bcid_end,
                    int bcid_truth
                    ):
  m_eta(eta),
  m_phi(phi),
  m_deta(deta),
  m_dphi(dphi),
  m_radius_min(rmin),
  m_radius_max(rmax),
  m_sampling(sampling),
  m_hash(hash),
  /* Bunch crossing information */
  m_bc_duration( bc_duration ),
  m_bc_nsamples( bc_nsamples ),
  m_bcid_start( bcid_start ),
  m_bcid_end( bcid_start ),
  m_bcid_truth( bcid_truth ),
  // Cell's information
  m_rawEnergy(0),
  m_truthRawEnergy(0),
  m_rawEnergySamples( (bcid_end-bcid_start)*bc_nsamples, 0 ),
{
  // Initalize the time vector using the bunch crossing informations
  float start = m_bcid_start * m_bc_duration;
  float step  = m_bc_duration / m_bc_nsamples;
  int total   = ((m_bcid_end - m_bcid_start) + 1) * m_bc_nsamples;
  for (int t = 0; t < total; ++t) m_time.push_back( (start + step*t) );
}



CaloCell::~CaloCell()
{;}



void CaloCell::clear()
{
  m_rawEnergy=0.0;
  m_truthRawEnergy=0.0;
  for (std::vector<float>::iterator it = m_rawEnergySamples.begin(); it < m_rawEnergySamples.end(); it++)
  {
    *it=0.0;
  }
}




void CaloCell::Fill( const G4Step* step )
{
  // Get total energy deposit
  float edep = (float)step->GetTotalEnergyDeposit();
  G4StepPoint* point = step->GetPreStepPoint();
  // Get the position
  G4ThreeVector pos = point->GetPosition();
  // Get the particle time
  float t = (float)point->GetGlobalTime()*mm/c_light; // mm to ns

  for(unsigned int sample=0; sample < m_rawEnergySamples.size(); ++sample){
    if( t >= m_time[sample] && t < m_time[sample+1]){
      m_rawEnergySamples[sample]+=edep;
      break;
    }
  }

  if ( t >= ( (m_bcid_truth-1)*bc_duration) && t < ((m_bcid_truth+1)*bc_duration)){
    m_truthRawEnergy+=edep;
  }else{
    m_rawEnergy+=edep;
  }
}







xAOD::CaloCell* CaloCell::copy()
{
  auto newcell = new xAOD::CaloCell();
  
  // Cell location parameters
  newcell->setEta(eta());
  newcell->setPhi(phi());
  newcell->setDeltaEta( deltaEta() );
  newcell->setDeltaPhi( deltaPhi() );
  newcell->setRmin( rmin() );
  newcell->setRmax( rmax() );
  newcell->setHash( hash() );
  newcell->setSampling(sampling());

  // Cell energy parameters
  newcell->setEt( et() );
  newcell->setEnergy( energy() );
  newcell->setRawEt( rawEt() );
  newcell->setRawEnergy( rawEnergy() );
  newcell->setTruthRawEnergy( truthRawEnergy() );
  newcell->setRawEnergySamples( rawEnergySamples() );

  // Bunch crossing parameters
  newcell->set_bcid_start( bcid_start() );
  newcell->set_bcid_end( bcid_end() );
  newcell->set_bcid_truth( bcid_truth() );
  newcell->set_bc_nsamples( bc_nsamples() );
  newcell->set_bc_duration( bc_duration() );

  // Pulse parameters
  newcell->setPulse( pulse() );
  newcell->setTime( time() );
  return newcell;
}


