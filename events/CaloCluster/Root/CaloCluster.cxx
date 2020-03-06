

#include "CaloCluster/CaloCluster.h"


using namespace xAOD;


CaloCluster::CaloCluster()
{;}

CaloCluster::CaloCluster( float emaxs2, float eta, float phi, 
                          float deta, float dphi):
  m_emaxs2(emaxs2),
  m_eta(eta),
  m_phi(phi),
  m_deta(deta),
  m_dphi(dphi)
{;}


CaloCluster::~CaloCluster()
{;}



void CaloCluster::clear()
{
  m_collection.clear();
}


void CaloCluster::push_back( const xAOD::CaloCell *cell )
{
	m_collection.push_back(cell);
}


size_t CaloCluster::size()
{ 
  return m_collection.size();
}


std::vector<const xAOD::CaloCell*> CaloCluster::all()
{
  return m_collection;
}


