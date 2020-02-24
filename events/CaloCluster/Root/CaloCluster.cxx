

#include "CaloCluster/CaloCluster.h"


using namespace xAOD;


CaloCluster::CaloCluster( float energy_center, float eta_center, 
                          float phi_center, float delta_eta, 
                          float delta_phi):
  m_energy_center(energy_center),
  m_eta_center(eta_center),
  m_phi_center(phi_center),
  m_delta_eta(delta_eta),
  m_delta_phi(delta_phi),
{;}


CaloCluster::~CaloCluster()
{;}


void CaloCluster::clear()
{
  m_collection.clear();
  m_totalEnergy=0;
}


const std::vector<xAOD::CaloCell*>& CaloCluster::getCollection()
{
  return m_collection;
}


void CaloCluster::push_back( xAOD::CaloCell *cell )
{
	m_collection.push_back(cell);
  m_totalEnergy+=cell->rawEnergy();
}


size_t CaloCluster::size()
{ 
  return m_collection.size();
}



