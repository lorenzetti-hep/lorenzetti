#include "CaloRings/CaloRings.h"

using namespace xAOD;
using namespace CaloSampling;


RingSet::RingSet( CaloSampling::CaloSample sampling, unsigned nrings, float deta, float dphi ):
  m_sampling(sampling), m_pattern(nrings,0), m_deta(deta), m_dphi(dphi)
{;}


void RingSet::add( const xAOD::CaloCell *cell , float eta_center, float phi_center )
{
  // This cell does not allow to this RingSet
  if( cell->sampling() != m_sampling )  return;
  float deta = std::abs( eta_center - cell->eta() ) / m_deta;
  float dphi = std::abs( phi_center - cell->phi() ) / m_dphi;
  float deltaGreater = std::max(deta, dphi);
  int i = static_cast<unsigned int>( std::floor(deltaGreater) );
  
  if( i < m_pattern.size() ){
    m_pattern[i] += cell->energy()/ std::cosh(std::abs(eta_center));
  }
}


size_t RingSet::size() const
{
  return m_pattern.size();
}


const std::vector<float>& RingSet::pattern() const
{
  return m_pattern;
}

CaloSampling::CaloSample RingSet::sampling() const
{
  return m_sampling;
}

void RingSet::clear()
{
  for (std::vector<float>::iterator it = m_pattern.begin(); it < m_pattern.end(); it++)
  {
    *it=0.0;
  }
}

