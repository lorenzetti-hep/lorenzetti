
#include "G4Kernel/CaloPhiRange.h"
#include "G4PhysicalConstants.hh"
#include <cmath>
#include <iostream>
//#include <iomanip>
const float CaloPhiRange::m_phi_min = -1*CLHEP::pi;
const float CaloPhiRange::m_twopi = CLHEP::twopi;
const float CaloPhiRange::m_phi_max = CLHEP::pi;


float CaloPhiRange::fix ( float phi )
{
  if (phi < m_phi_min) return (phi+m_twopi);
  if (phi > m_phi_max) return (phi-m_twopi);
  return phi;
}


float CaloPhiRange::diff ( float phi1, float phi2 )
{
  float res = fix(phi1) - fix(phi2);
  return fix(res);
}


