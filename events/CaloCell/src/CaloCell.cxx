
#include "CaloCell/CaloCell.h"
#include "CaloCell/CaloDetDescriptor.h"
#include "CaloCell/enumeration.h"

using namespace xAOD;



CaloCell::CaloCell( float eta, 
                    float phi, 
                    float deta, 
                    float dphi):
  EDM(),
  m_eta(eta),
  m_phi(phi),
  m_deta(deta),
  m_dphi(dphi)
{;}


const xAOD::CaloDetDescriptor* CaloCell::descriptor() const
{
  return m_descriptor;
}


void CaloCell::setDescriptor( const xAOD::CaloDetDescriptor *descriptor )
{
  m_descriptor=descriptor;
}



