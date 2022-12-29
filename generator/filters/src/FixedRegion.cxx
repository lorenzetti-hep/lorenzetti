#include "FixedRegion.h"

using namespace generator;

FixedRegion::FixedRegion(const std::string name):
  IMsgService(name),
  IAlgorithm(nullptr)
{
  declareProperty( "Eta"            , m_eta                 );
  declareProperty( "Phi"            , m_phi                 );
}



StatusCode FixedRegion::initialize()
{
  return StatusCode::SUCCESS;
}



StatusCode FixedRegion::execute( generator::Event& event )
{
  event.setEventNumber(-1);
  auto seed = generator::Seed(m_eta,m_phi);
  event.push_back(seed);
  return StatusCode::SUCCESS;
}



StatusCode FixedRegion::finalize()
{
  return StatusCode::SUCCESS;
}


