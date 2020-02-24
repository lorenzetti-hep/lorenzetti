

#include "CaloCluster/CaloClusterContainer.h"

using namespace xAOD;

CaloClusterContainer::CaloClusterContainer()
{;}


CaloClusterContainer::~CaloClusterContainer()
{
  clear();
}


void CaloClusterContainer::push_back( xAOD::CaloCluster *caloCluster )
{
  m_container.push_back(caloCluster);
}


size_t CaloClusterContainer::size()
{
  return m_container.size();
}


void CaloClusterContainer::clear()
{
  for ( auto& caloCluster : m_container ){
    if( caloCluster )
      delete caloCluster;
  }

  m_container.clear();
}








