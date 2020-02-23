
#include "edm/CaloCell.hh"
#include "edm/enumeration.hh"
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

  m_eta_center(eta_center),
  m_phi_center(phi_center),
  m_delta_eta(delta_eta),
  m_delta_phi(delta_phi),
  m_rmin(rmin),
  m_rmax(rmax),
  m_sampling(sampling),
  m_tbins(tbins),
  m_rawEnergySamples( tbins.size()-1, 0 ),
  m_rawEnergy(0),
  m_hash(hash)
{;}



xAOD::CaloCell* CaloCell::clone()
{
  return new xAOD::CaloCell( eta(), phi(), 
                            deltaEta(), deltaPhi(),
                            rmin(),rmax(),
                            sampling(), m_tbins, m_hash);

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


void CaloCell::push_back( const G4Step* step )
{
  // Get total energy deposit
  float edep = (float)step->GetTotalEnergyDeposit();
  G4StepPoint* point = step->GetPreStepPoint();
  // Get the position
  G4ThreeVector pos = point->GetPosition();
  // Get the particle time
  float t = (float)point->GetGlobalTime()/c_light;
  //float t1 = (float)pos.t()/c_light;
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







/*
 * Halper class:
 * Cell Accessor to generate the correct hash given eta/phi and r
 */

CaloCellAccessor::CaloCellAccessor( float etamin, float etamax, float etabins, float phimin, 
                    float phimax, float phibins, float rmin, float rmax, int sampling):
  m_rmin(rmin), m_rmax(rmax), m_sampling(sampling)
{
  float deta = (etamax-etamin)/etabins;
  float dphi = (phimax-phimin)/phibins;
  for (unsigned eta_idx=0 ; eta_idx<etabins+1; ++eta_idx)
    m_eta_bins.push_back( etamin + deta * eta_idx );
  for (unsigned phi_idx=0 ; phi_idx<phibins+1; ++phi_idx)
    m_phi_bins.push_back( phimin + dphi * phi_idx );

}


bool CaloCellAccessor::isValid( float r ){
  return  ( r >= m_rmin && r < m_rmax ) ? true : false;
}



bool CaloCellAccessor::getHash( float eta, float phi , std::string &cell_hash)
{
  std::stringstream ss;
  for ( unsigned eta_bin = 0 ; eta_bin < m_eta_bins.size()-1; ++eta_bin ){
    if ( eta > m_eta_bins[eta_bin] && eta <= m_eta_bins[eta_bin+1]){
      for ( unsigned phi_bin = 0 ; phi_bin < m_phi_bins.size()-1; ++phi_bin ){
        if ( phi > m_phi_bins[phi_bin] && phi <= m_phi_bins[phi_bin+1]){
          ss << "layer" << m_sampling << "_eta" << eta_bin << "_phi" << phi_bin;
          cell_hash = ss.str();
          return true;
        }
      }
    }
  }
  return false;
}






