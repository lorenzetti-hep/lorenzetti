#include "Cell.h"
#include "EventInfo/EventInfo.h"

using namespace Pythia8;


Cell::Cell():
  IMsgService("Cell"),
  Physics()
{
  declareProperty( "Eta"            , m_eta                 );
  declareProperty( "Phi"            , m_phi                 );
  //declareProperty( "Sampling"       , m_sampling            );
  declareProperty( "OutputLevel"    , m_outputLevel=1       );
}



StatusCode Cell::initialize()
{
  return StatusCode::SUCCESS;
}



StatusCode Cell::run( std::vector<xAOD::seed_t> &seed_vec,  std::vector<std::vector<Particle*>> &particles )
{
  std::vector<Particle*> vec;
  // Add any particles
  particles.push_back( vec );
  seed_vec.push_back( xAOD::seed_t{ 0.0, m_eta, m_phi, -1., -1., -1, 0 } );
  
  return StatusCode::SUCCESS;
}



StatusCode Cell::finalize()
{
  return StatusCode::SUCCESS;
}


