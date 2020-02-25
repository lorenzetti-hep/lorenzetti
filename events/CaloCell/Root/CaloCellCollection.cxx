

#include "CaloCell/CaloCellCollection.h"

using namespace xAOD;


CaloCellCollection::CaloCellCollection()
{;}


CaloCellCollection::~CaloCellCollection()
{
  for ( auto& pairObj : m_collection ){
    if( pairObj.second )
      delete pairObj.second;
  }

  for (auto& acc : m_collectionAccessor ){
    if(acc)
      delete acc;
  }
}


void CaloCellCollection::push_back( xAOD::CaloCell *cell )
{
  m_collection[ cell->hash() ] = cell;
}



void CaloCellCollection::push_back( xAOD::CaloCellAccessor *acc )
{
  m_collectionAccessor.push_back(acc);
}



bool CaloCellCollection::retrieve( TLorentzVector &pos, xAOD::CaloCell *&cell )
{
  // Apply all necessary transformation (x,y,z) to (eta,phi,r) coordinates
  // Get ATLAS coordinates (in transverse plane xy)
  float eta = pos.Eta(); 
  float phi = pos.Phi(); 
  float r = abs(pos.Mag());

  for ( auto& acc : m_collectionAccessor )
  {
    std::string cell_hash;
    if( acc->getHash( eta, phi, r, cell_hash ))
    {
      cell = m_collection[cell_hash];
      return true;
    }
  }
  return false;
}


size_t CaloCellCollection::size()
{
  return m_collection.size();
}


void CaloCellCollection::clear()
{
  for ( auto& pairObj : m_collection ){
    pairObj.second->clear();
  }
}


// Get all calocells given a layer
std::vector< xAOD::CaloCell* > CaloCellCollection::getCollection( CaloSampling::CaloSample sampling )
{
  std::vector< xAOD::CaloCell* > vec;
  
  for ( auto& pairObj : m_collection ){
    auto *cell = pairObj.second;
    if( cell->sampling() == sampling )
      vec.push_back( cell );
  }
  return vec;
}


// Get all calocells 
std::vector< xAOD::CaloCell* > CaloCellCollection::getCollection()
{
  std::vector< xAOD::CaloCell* > vec;
  for ( auto& pairObj : m_collection ){
    auto *cell = pairObj.second;
    vec.push_back( cell );
  }
  return vec;
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


bool CaloCellAccessor::getHash( float eta, float phi, float r, std::string &cell_hash)
{
  
  if( !(r >= m_rmin && r < m_rmax) )
    return false;


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






