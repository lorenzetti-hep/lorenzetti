#include <iostream>
#include "CaloCellCollection.h"
#include "GaugiKernel/Timer.h"

using namespace xAOD;
using namespace CaloSampling;


CaloCellCollection::CaloCellCollection( float etamin, float etamax, float etabins, float phimin, float phimax, 
                                        float phibins,float rmin,   float rmax, CaloSample sampling):
  m_radius_min(rmin), 
  m_radius_max(rmax), 
  m_sampling(sampling), 
  m_etamin(etamin), 
  m_etamax(etamax),
  m_phimin(phimin),
  m_phimax(phimax)
{
  float deta = (etamax-etamin)/etabins;
  float dphi = (phimax-phimin)/phibins;
  for (unsigned eta_idx=0 ; eta_idx<etabins+1; ++eta_idx){
    m_eta_bins.push_back( etamin + deta * eta_idx );
  }
    
  for (unsigned phi_idx=0 ; phi_idx<phibins+1; ++phi_idx) {
    m_phi_bins.push_back( phimin + dphi * phi_idx );
  }

}


CaloCellCollection::~CaloCellCollection()
{
  for(auto& p : m_collection)
    if(p.second)  delete p.second;
  m_collection.clear();
}


CaloSample CaloCellCollection::sampling() const
{
  return m_sampling;
}



void CaloCellCollection::push_back( xAOD::RawCell *cell )
{
  m_collection.insert( std::make_pair( cell->hash(), cell ) );
}


bool CaloCellCollection::retrieve( TVector3 &pos, xAOD::RawCell *&cell ) const
{
  // Retrun nullptr in case of not match
  cell = nullptr;
  
  // Apply all necessary transformation (x,y,z) to (eta,phi,r) coordinates
  // Get ATLAS coordinates (in transverse plane xy)
  float eta = pos.PseudoRapidity();
  float phi = pos.Phi();
  float radius = pos.Perp();

  // In plan xy
  if( !(radius > m_radius_min && radius <= m_radius_max) )
    return false;

  if( !(eta > m_etamin && eta <= m_etamax) )
    return false;

  if( !(phi > m_phimin && phi <= m_phimax) )
    return false;


  int etaBin = findIndex( m_eta_bins, eta );
  int phiBin = findIndex( m_phi_bins, phi );
  
  if (etaBin!=-1 && phiBin!=-1){
    unsigned int hash = (int)m_sampling*1e7 + ( etaBin*(m_phi_bins.size()-1) + phiBin );
    cell = m_collection.at(hash);
    return true;
  }



  return false;
}


size_t CaloCellCollection::size() const
{
  return m_collection.size();
}


const CaloCellCollection::collection_map_t& CaloCellCollection::operator*() const
{
  return m_collection;
}


int CaloCellCollection::findIndex( const std::vector<float> &vec, float value) const 
{
  auto binIterator = std::adjacent_find( vec.begin(), vec.end(), [=](float left, float right){ return left < value and value <= right; }  );
  if ( binIterator == vec.end() ) return -1;
  return  binIterator - vec.begin();
}




