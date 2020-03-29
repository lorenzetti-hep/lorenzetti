

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


std::vector<const xAOD::CaloCell*> CaloCluster::allCells()
{
  return m_container;
}


void CaloCluster::Print()
{
  MSG_INFO( "Et       : " << et() );
  MSG_INFO( "Eta      : " << eta() );
  MSG_INFO( "Phi      : " << phi() );
  MSG_INFO( "e0       : " << e0() );
  MSG_INFO( "e1       : " << e1() );
  MSG_INFO( "e2       : " << e2() );
  MSG_INFO( "e3       : " << e3() );
  MSG_INFO( "f0       : " << f0() );
  MSG_INFO( "f1       : " << f1() );
  MSG_INFO( "f2       : " << f2() );
  MSG_INFO( "f3       : " << f3() );
  MSG_INFO( "rhad     : " << rhad() );
  MSG_INFO( "ehad1    : " << ehad1() );
  MSG_INFO( "ehad2    : " << ehad2() );
  MSG_INFO( "ehad3    : " << ehad3() );
  MSG_INFO( "emaxs1   : " << emaxs1() );
  MSG_INFO( "emaxs2   : " << emaxs2() );
  MSG_INFO( "e2tsts1  : " << e2tsts1() );
  MSG_INFO( "reta     : " << reta() );
  MSG_INFO( "rphi     : " << rphi() );
  MSG_INFO( "eratio   : " << eratio() );
  MSG_INFO( "e233     : " << e233() );
  MSG_INFO( "e237     : " << e237() );
  MSG_INFO( "e277     : " << e277() );
}








