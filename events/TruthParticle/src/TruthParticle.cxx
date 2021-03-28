

#include "TruthParticle/TruthParticle.h"


using namespace xAOD;


TruthParticle::TruthParticle():
  m_pdgid(0),
  m_e(0),
  m_et(0),
  m_eta(0),
  m_phi(0)
{;}


TruthParticle::TruthParticle( float e, float et, float eta, float phi , int pdgid ): 
  m_pdgid(pdgid),
  m_e(e),
  m_et(et),
  m_eta(eta),
  m_phi(phi)
{;}











