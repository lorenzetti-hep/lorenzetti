
#include "CaloCell/CaloCell.h"
#include "CaloCell/enumeration.h"

using namespace xAOD;
using namespace CaloSampling;


CaloCell::CaloCell( float eta, 
                    float phi, 
                    float deta, 
                    float dphi, 
                    float radius_min, 
                    float radius_max,
                    std::string hash,
                    CaloSample sampling ):
  m_eta(eta),
  m_phi(phi),
  m_deta(deta),
  m_dphi(dphi),
  m_radius_min(radius_min),
  m_radius_max(radius_max),
  m_sampling(sampling)
{;}


const xAOD::RawEnergy* CaloCell::parent() const
{
  return m_parent;
}


void CaloCell::setParent( const xAOD::RawEnergy *parent )
{
  m_parent=parent;
}


