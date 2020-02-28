
#include "CaloCell/CaloCell.h"
#include "core/enumeration.h"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"


using namespace xAOD;



CaloCell::CaloCell( float eta_center, 
                    float phi_center, 
                    float delta_eta, 
                    float delta_phi, 
                    float rmin, 
                    float rmax,
                    CaloSampling::CaloSample sampling, 
                    std::vector<float> tbins,
                    std::string hash): 

  m_sampling(sampling),
  m_eta_center(eta_center),
  m_phi_center(phi_center),
  m_delta_eta(delta_eta),
  m_delta_phi(delta_phi),
  m_rmin(rmin),
  m_rmax(rmax),
  m_rawEnergy(0),
  m_rawEnergySamples( tbins.size()-1, 0 ),
  m_tbins(tbins),
  m_hash(hash)
{;}



xAOD::CaloCell* CaloCell::clone()
{
  return new xAOD::CaloCell( eta(), phi(), 
                            deltaEta(), deltaPhi(),
                            rmin(),rmax(),
                            sampling(), layer(), m_tbins, m_hash);

}


CaloCell::~CaloCell()
{;}



void CaloCell::clear()
{
  m_rawEnergy=0.0;
  for (std::vector<float>::iterator it = m_rawEnergySamples.begin(); it < m_rawEnergySamples.end(); it++)
  {
    *it=0.0;
  }
}



CaloSampling::CaloLayer CaloCell::layer()
{
  return ((int)m_sampling < 0) ? CaloSampling::CaloLayer::LAr : CaloSampling::CaloLayer::Tile ;
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


  int bin=-1;
  for(unsigned int sample=0; sample < m_rawEnergySamples.size(); ++sample){
    if( t >= m_tbins[sample] && t < m_tbins[sample+1]){
      bin=sample; break;
    }
  }
  if(bin>=0)  
    m_rawEnergySamples[bin]+=edep;

  m_rawEnergy+=edep;
}







