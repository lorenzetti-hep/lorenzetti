

#include "Truth/Truth.h"


using namespace xAOD;


Truth::Truth():
  m_et(0),
  m_eta(0),
  m_phi(0),
  m_pdgid(-1),
  m_cluster(nullptr)
{}

Truth::Truth( float et, float eta, float phi , int pdgid): 
  m_et(et),
  m_eta(eta),
  m_phi(phi),
  m_pdgid(pdgid),
  m_cluster(nullptr)
{;}


Truth::~Truth()
{;}





