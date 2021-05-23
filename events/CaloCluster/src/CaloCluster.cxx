

#include "CaloCluster/CaloCluster.h"
//#include "G4Kernel/macros.h"


using namespace xAOD;


CaloCluster::CaloCluster()
{;}

CaloCluster::CaloCluster( float emaxs2, float eta, float phi, 
                          float deta, float dphi):
  EDM(),
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
  m_container.clear();
}


void CaloCluster::push_back( const xAOD::CaloCell *cell )
{
	m_container.push_back(cell);
}


size_t CaloCluster::size()
{ 
  return m_container.size();
}


const std::vector<const xAOD::CaloCell*>& CaloCluster::cells() const
{
  return m_container;
}


