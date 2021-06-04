#include <iostream>
#include "CaloCellCollection.h"
#include "GaugiKernel/Timer.h"

using namespace xAOD;


CaloCellCollection::CaloCellCollection( float etamin, float etamax, 
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


CaloCellCollection::~CaloCellCollection()
{
  for(auto& p : m_collection)
    if(p.second)  delete p.second;
  m_collection.clear();
}


CaloSampling CaloCellCollection::sampling() const
{
  return m_sampling;
}



void CaloCellCollection::push_back( xAOD::CaloDetDescriptor *cell )
{
  m_collection.insert( std::make_pair( cell->hash(), cell ) );
}


bool CaloCellCollection::retrieve( const xAOD::CaloHit *hit , xAOD::CaloDetDescriptor *&cell ) const
{
  // Retrun nullptr in case of not match
  cell = nullptr;
  
  // sampling 
  if ( hit->sampling() != m_sampling )
    return false;

  float eta = hit->eta();
  float phi = hit->phi();

  // segmentation
  if( !(abs(eta) > m_etamin && abs(eta) <= m_etamax) )
    return false;

  int etaBin = findIndex( m_eta_bins, eta );
  int phiBin = findIndex( m_phi_bins, phi );
  
  if (etaBin!=-1 && phiBin!=-1){    
    unsigned int hash = m_detector*1e9 + m_sampling*1e7 + m_segmentation*1e6 + ( etaBin*(m_phi_bins.size()-1) + phiBin );
    if (m_collection.count(hash)>0){
      cell = m_collection.at(hash);
      return true;
    }else{
      std::cout << "error! hash " << hash << " does not exist into the map collection but the bin " <<  
      "grid find some one. Let's skip this..." << std::endl;
    }
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




