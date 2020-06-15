#include "P8Kernel/Algorithm.h"

using namespace Gaugi;
using namespace generator;

static const double c_light = 2.99792458e+8; // m/s


Algorithm::Algorithm():
  IMsgService(),
  PropertyService()
{
  declareProperty("Seed"        , m_seed=0                                                                );
  declareProperty("OutputLevel" , m_outputLevel=0                                                         );
  declareProperty( "Sigma_t"    , m_sigma_t= 200 * 1e-12 /*pico seconds*/ * c_light /*m/s*/ * 1e+3 /*mm*/ );
  declareProperty( "Sigma_z"    , m_sigma_z=56 /*miliseconds*/                                            );
 
}


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


