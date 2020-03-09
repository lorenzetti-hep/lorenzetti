
#include "CaloCell/CaloCell.h"
#include "core/enumeration.h"
#include "core/constants.h"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"


using namespace xAOD;

CaloCell::CaloCell()
{;}


CaloCell::CaloCell( float eta, 
                    float phi, 
                    float deta, 
                    float dphi, 
                    float rmin, 
                    float rmax,
                    CaloSampling::CaloSample sampling, 
                    std::vector<float> t,
                    std::string hash): 

  m_sampling(sampling),
  m_eta(eta),
  m_phi(phi),
  m_deta(deta),
  m_dphi(dphi),
  m_rmin(rmin),
  m_rmax(rmax),
  m_rawEnergy(0),
  m_truthRawEnergy(0),
  m_rawEnergySamples( t.size()-1, 0 ),
  m_t(t),
  m_hash(hash)
{;}



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



CaloSampling::CaloLayer CaloCell::layer()
{
  return ((int)m_sampling < 4) ? CaloSampling::CaloLayer::ECal : CaloSampling::CaloLayer::HCal;
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

  float bc_duration = abs(m_t[1]-m_t[0]);

  int bin=-1;
  for(unsigned int sample=0; sample < m_rawEnergySamples.size(); ++sample){
    if( t >= m_t[sample] && t < m_t[sample+1]){
      bin=sample; break;
    }
  }

  if(bin>=0)
    m_rawEnergySamples[bin]+=edep;
  
  // Accumulate the truth energy bc+-15ns
  if ( t >= (BC_TRUTH_EVENT*bc_duration - 20) && t < (BC_TRUTH_EVENT*bc_duration) + 20 ){
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
  newcell->setEt( et() );
  newcell->setEnergy( energy() );
  newcell->setRawEt( rawEt() );
  newcell->setRawEnergy( rawEnergy() );
  newcell->setTruthRawEnergy( truthRawEnergy() );
  newcell->setRawEnergySamples( rawEnergySamples() );
  newcell->setPulse( pulse() );
  newcell->setSampling(sampling());
  newcell->setT( t() );
  return newcell;
}


