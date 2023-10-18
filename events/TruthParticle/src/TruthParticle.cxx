

#include "TruthParticle/TruthParticle.h"


using namespace xAOD;


TruthParticle::TruthParticle():
  m_pdgid(0),
  m_seedid(0),
  m_e(0),
  m_et(0),
  m_eta(0),
  m_phi(0),
  m_px(0),
  m_py(0),
  m_pz(0),
  m_vtx_x(0),
  m_vtx_y(0),
  m_vtx_z(0)
{;}


TruthParticle::TruthParticle( int seedid, int pdgid, 
                              float e, float et, float eta, float phi , 
                              float px, float py, float pz,  
                              float vx, float vy, float vz ):
                                
  m_pdgid(pdgid),
  m_seedid(seedid),
  m_e(e),
  m_et(et),
  m_eta(eta),
  m_phi(phi),
  m_px(px),
  m_py(py),
  m_pz(pz),
  m_vtx_x(vx),
  m_vtx_y(vy),
  m_vtx_z(vz)
{;}











