#include "FixedRegion.h"

using namespace Pythia8;
using namespace generator;

FixedRegion::FixedRegion():
  IMsgService("FixedRegion"),
  Algorithm()
{
  declareProperty( "Eta"            , m_eta                 );
  declareProperty( "Phi"            , m_phi                 );
}


FixedRegion::~FixedRegion()
{;}


StatusCode FixedRegion::initialize()
{
  return StatusCode::SUCCESS;
}



StatusCode FixedRegion::execute( generator::Event& event )
{
  
  auto seed = generator::Seed(m_eta,m_phi);
  event.push_back(seed);
  return StatusCode::SUCCESS;
}



StatusCode FixedRegion::finalize()
{
  return StatusCode::SUCCESS;
}


