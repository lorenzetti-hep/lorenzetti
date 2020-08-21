
#include "CaloCell/CaloCell.h"
#include "CaloCell/RawCell.h"
#include "CaloCell/enumeration.h"

using namespace xAOD;
using namespace CaloSampling;


CaloCell::CaloCell( float eta, 
                    float phi, 
                    float deta, 
                    float dphi, 
                    CaloSample  sample,
                    CaloLayer   layer,
                    CaloSection section
                    ):
  m_sample(sample),
  m_layer(layer),
  m_section(section),
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



