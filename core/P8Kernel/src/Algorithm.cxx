#include "P8Kernel/Algorithm.h"

using namespace Gaugi;
using namespace generator;

Algorithm::Algorithm():
  IMsgService(),
  PropertyService()
{;}


int Algorithm::poisson(double nAvg) 
{
  // Set maximum to avoid overflow.
  const int nmax = 300;
  // Random number.
  double rPoisson = m_generator.rndm.flat() * exp(nAvg);
  // Initialize.
  double rSum  = 0.;
  double rTerm = 1.;
  // Add to sum and check whether done.
  for (int i = 0; i < nmax; ) {
    rSum += rTerm;
    if (rSum > rPoisson) return i;
    // Evaluate next term.
    ++i;
    rTerm *= nAvg / i;
  }
  // Emergency return.
  return nmax;
}


const std::string& Algorithm::name() const
{
  return getLogName();
}


