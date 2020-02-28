

#include "Truth/Truth.h"


using namespace xAOD;


Truth::Truth():
  m_energy(0),
  m_eta(0),
  m_phi(0),
  m_pdgid(-1)

Truth::Truth( float energy, float eta, float phi , int pdgid): 
  m_energy(energy),
  m_eta(eta),
  m_phi(phi),
  m_pdgid(pdgid),
  m_caloCluster(nullptr)
{;}


Truth::~Truth()
{;}





