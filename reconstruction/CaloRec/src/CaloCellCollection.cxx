
#include "CaloCellCollection.h"

using namespace xAOD;
using namespace CaloSampling;

CaloCellCollection::CaloCellCollection( float etamin, 
                                        float etamax, 
                                        float etabins, 
                                        float phimin, 
                                        float phimax, 
                                        float phibins, 
                                        float rmin, 
                                        float rmax, 
                                        CaloSample sampling
                                        ):
  m_radius_min(rmin), m_radius_max(rmax), m_sampling(sampling)
{
  float deta = (etamax-etamin)/etabins;
  float dphi = (phimax-phimin)/phibins;
  for (unsigned eta_idx=0 ; eta_idx<etabins+1; ++eta_idx) m_eta_bins.push_back( etamin + deta * eta_idx );
  for (unsigned phi_idx=0 ; phi_idx<phibins+1; ++phi_idx) m_phi_bins.push_back( phimin + dphi * phi_idx );
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


void CaloCellCollection::push_back( xAOD::CaloCell *cell )
{
  // This collection will be responsible to manager the memory allocated for each cell
  //m_collection.insert( std::make_pair( cell->hash(), std::unique_ptr<xAOD::CaloCell>(cell) ) );
  m_collection.insert( std::make_pair( cell->hash(), cell ) );
}



bool CaloCellCollection::retrieve( TVector3 &pos, xAOD::CaloCell *&cell ) const
{
  // Retrun nullptr in case of not match
  cell = nullptr;
  // Apply all necessary transformation (x,y,z) to (eta,phi,r) coordinates
  // Get ATLAS coordinates (in transverse plane xy)
  float eta = pos.PseudoRapidity();
  float phi = pos.Phi();
  float radius = pos.Perp();
  std::string hash;

  // In plan xy
  if( !(radius >= m_radius_min && radius < m_radius_max) )
    return false;

  // Try to find the correct eta/phi bin for this position
  for ( unsigned eta_bin = 0 ; eta_bin < m_eta_bins.size()-1; ++eta_bin ){
    if ( eta > m_eta_bins[eta_bin] && eta <= m_eta_bins[eta_bin+1]){
      for ( unsigned phi_bin = 0 ; phi_bin < m_phi_bins.size()-1; ++phi_bin ){
        if ( phi > m_phi_bins[phi_bin] && phi <= m_phi_bins[phi_bin+1]){
          std::stringstream ss;
          ss << "layer" << (int)m_sampling << "_eta" << eta_bin << "_phi" << phi_bin;
          //cell = m_collection[ss.str()].get();
          cell = m_collection.at(ss.str());
          return true;
        }
      }
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


float CaloCellCollection::totalEnergy() const 
{
  float total=0;
  for(auto& p : m_collection)
    if(p.second) total+=p.second->rawEnergy();
  return total;
}



