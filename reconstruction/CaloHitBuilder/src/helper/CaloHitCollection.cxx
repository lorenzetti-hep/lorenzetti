
#include <iostream>
#include "CaloHitCollection.h"
#include "GaugiKernel/Timer.h"

using namespace xAOD;


CaloHitCollection::CaloHitCollection( float etamin, float etamax, 
                                      std::vector<float> etabins, 
                                      std::vector<float> phibins, 
                                      float rmin,   float rmax, 
                                      Detector detector, // Can be TTEM, TTHEC, TILE, ...
                                      CaloSampling sampling, // Can be EMEC1, PSB, TileExt1, ...
                                      unsigned segmentation):

  m_eta_bins(etabins),
  m_phi_bins(phibins),
  m_radius_min(rmin), 
  m_radius_max(rmax), 
  m_segmentation(segmentation),
  m_sampling(sampling), 
  m_detector(detector),
  m_etamin(etamin), 
  m_etamax(etamax)
{;}


CaloHitCollection::~CaloHitCollection()
{
  for(auto& p : m_collection)
    if(p.second)  delete p.second;
  m_collection.clear();
}


CaloSampling CaloHitCollection::sampling() const
{
  return m_sampling;
}



void CaloHitCollection::push_back( xAOD::CaloHit *hit )
{
  m_collection.insert( std::make_pair( hit->hash(), hit ) );
}


bool CaloHitCollection::retrieve( TVector3 &pos , xAOD::CaloHit *&hit) const
{
  // Retrun nullptr in case of not match
  hit= nullptr;
  
  // Apply all necessary transformation (x,y,z) to (eta,phi,r) coordinates
  // Get ATLAS coordinates (in transverse plane xy)
  float eta = pos.PseudoRapidity();
  float phi = pos.Phi();
  float radius = pos.Perp();

  // In plan xy
  if( !(radius > m_radius_min && radius <= m_radius_max) )
    return false;

  // segmentation
  if( !(abs(eta) > m_etamin && abs(eta) <= m_etamax) )
    return false;

  int etaBin = findIndex( m_eta_bins, eta );
  int phiBin = findIndex( m_phi_bins, phi );
  
  if (etaBin!=-1 && phiBin!=-1){    
    unsigned long int hash = m_detector*1e9 + m_sampling*1e7 + m_segmentation*1e6 + ( etaBin*(m_phi_bins.size()-1) + phiBin );
    if (m_collection.count(hash)>0){
      hit = m_collection.at(hash);
      return true;
    }else{
      std::cout << "error! hash " << hash << " does not exist into the map collection but the bin " <<  
      "grid find some one. Let's skip this..." << std::endl;
    }
  }

  return false;
}


size_t CaloHitCollection::size() const
{
  return m_collection.size();
}


const CaloHitCollection::collection_map_t& CaloHitCollection::operator*() const
{
  return m_collection;
}


int CaloHitCollection::findIndex( const std::vector<float> &vec, float value) const 
{
  auto binIterator = std::adjacent_find( vec.begin(), vec.end(), [=](float left, float right){ return left < value and value <= right; }  );
  if ( binIterator == vec.end() ) return -1;
  return  binIterator - vec.begin();
}




