
#include "CaloCell/CaloCell.h"
#include "CaloCell/RawCell.h"
#include "CaloCell/enumeration.h"

using namespace xAOD;
using namespace CaloSampling;


CaloCell::CaloCell( float eta, 
                    float phi, 
                    float deta, 
                    float dphi, 
                    CaloSample sampling ):
  m_sampling(sampling),
  m_eta(eta),
  m_phi(phi),
  m_deta(deta),
  m_dphi(dphi)
{;}


const xAOD::RawCell* CaloCell::parent() const
{
  return m_parent;
}


void CaloCell::setParent( const xAOD::RawCell *parent )
{
  m_parent=parent;
}


CaloLayer CaloCell::detector() const
{
  if(sampling()==CaloSample::EM1 || sampling()==CaloSample::EM2 || sampling()==CaloSample::EM3)
    return CaloLayer::ECal;
  else // HAD1, HAD2 or HAD3
    return CaloLayer::HCal;
}

