#include "Seed.h"
#include "EventInfo/EventInfo.h"

using namespace Pythia8;


Seed::Seed():
  IMsgService("Seed"),
  Physics()
{
  declareProperty( "Eta"            , m_eta                 );
  declareProperty( "Phi"            , m_phi                 );
  declareProperty( "OutputLevel"    , m_outputLevel=1       );
}



StatusCode Seed::initialize()
{
  return StatusCode::SUCCESS;
}



StatusCode Seed::run( std::vector<xAOD::seed_t> &seed_vec,  std::vector<std::vector<Particle*>> &particles )
{
  std::vector<Particle*> vec;
  // Add any particles
  particles.push_back( vec );
  // One seed with any particle
  seed_vec.push_back( xAOD::seed_t{ 0.0, m_eta, m_phi, -1., -1., -1, 0 } );
  return StatusCode::SUCCESS;
}



StatusCode Seed::finalize()
{
  return StatusCode::SUCCESS;
}


