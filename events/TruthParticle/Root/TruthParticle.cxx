

#include "TruthParticle/TruthParticle.h"


using namespace xAOD;


TruthParticle::TruthParticle():
  m_eta(0),
  m_phi(0),
  m_pdgid(0),
  m_caloCluster(nullptr)
{;}

TruthParticle::TruthParticle( float eta, float phi , int pdgid): 
  m_eta(eta),
  m_phi(phi),
  m_pdgid(pdgid),
  m_caloCluster(nullptr)
{;}


TruthParticle::~TruthParticle()
{;}



void TruthParticle::clear()
{
  m_container.clear();
}


void TruthParticle::push_back( const xAOD::CaloCell *cell )
{
	m_container.push_back(cell);
}


size_t TruthParticle::size()
{ 
  return m_container.size();
}


std::vector<const xAOD::CaloCell*> TruthParticle::allCells()
{
  return m_container;
}



void TruthParticle::Print()
{
  MSG_INFO( "Et       : " << et() );
  MSG_INFO( "Eta      : " << eta() );
  MSG_INFO( "Phi      : " << phi() );
  MSG_INFO( "Reta     : " << reta() );
  MSG_INFO( "Rphi     : " << rphi() );
}










