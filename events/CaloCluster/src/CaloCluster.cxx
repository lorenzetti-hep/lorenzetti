

#include "CaloCluster/CaloCluster.h"
//#include "G4Kernel/macros.h"


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
  /*
  G4_MSG_VERBOSE( "Et       : " << et() );
  G4_MSG_VERBOSE( "Eta      : " << eta() );
  G4_MSG_VERBOSE( "Phi      : " << phi() );
  G4_MSG_VERBOSE( "e0       : " << e0() );
  G4_MSG_VERBOSE( "e1       : " << e1() );
  G4_MSG_VERBOSE( "e2       : " << e2() );
  G4_MSG_VERBOSE( "e3       : " << e3() );
  G4_MSG_VERBOSE( "f0       : " << f0() );
  G4_MSG_VERBOSE( "f1       : " << f1() );
  G4_MSG_VERBOSE( "f2       : " << f2() );
  G4_MSG_VERBOSE( "f3       : " << f3() );
  G4_MSG_VERBOSE( "rhad     : " << rhad() );
  G4_MSG_VERBOSE( "ehad1    : " << ehad1() );
  G4_MSG_VERBOSE( "ehad2    : " << ehad2() );
  G4_MSG_VERBOSE( "ehad3    : " << ehad3() );
  G4_MSG_VERBOSE( "emaxs1   : " << emaxs1() );
  G4_MSG_VERBOSE( "emaxs2   : " << emaxs2() );
  G4_MSG_VERBOSE( "e2tsts1  : " << e2tsts1() );
  G4_MSG_VERBOSE( "reta     : " << reta() );
  G4_MSG_VERBOSE( "rphi     : " << rphi() );
  G4_MSG_VERBOSE( "eratio   : " << eratio() );
  G4_MSG_VERBOSE( "e233     : " << e233() );
  G4_MSG_VERBOSE( "e237     : " << e237() );
  G4_MSG_VERBOSE( "e277     : " << e277() );
  */
}








