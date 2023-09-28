

#include "Seed/Seed.h"


using namespace xAOD;


Seed::Seed():
  m_id(-1),
  m_e(0),
  m_et(0),
  m_eta(0),
  m_phi(0)
{;}


Seed::Seed( int id, float e, float et, float eta, float phi  ): 
  m_id(id),
  m_e(e),
  m_et(et),
  m_eta(eta),
  m_phi(phi)
{;}











