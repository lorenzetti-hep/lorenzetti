#include "PulseGenerator.h"
#include "TPulseGenerator.h"

using namespace Gaugi;


PulseGenerator::PulseGenerator( std::string name ) : 
  CaloRecTool( name )
{
  declareProperty( "NSamples"     , m_nsamples=7              );
  declareProperty( "ShaperFile"   , m_shaperFile              );
  declareProperty( "OutputLevel"  , m_outputLevel = MSG::INFO );
}



PulseGenerator::~PulseGenerator()
{
  delete m_pulseGenerator;
}


StatusCode PulseGenerator::initialize()
{
  setMsgLevel( (MSG::Level)m_outputLevel );
  m_pulseGenerator = new CPK::TPulseGenerator( m_nsamples, m_shaperFile.c_str());
  return StatusCode::SUCCESS;
}


StatusCode PulseGenerator::finalize()
{
  return StatusCode::SUCCESS;
}


StatusCode PulseGenerator::executeTool( xAOD::CaloCell *cell ) const
{
  auto pulse_size = m_pulseGenerator->GetPulseSize();
  
  // Get all energies for each bunch crossing 
  auto rawEnergySamples = cell->rawEnergySamples();
  
  // Create an pulse with zeros with n samples
  std::vector<float> pulse_sum(pulse_size, 0.0);
  // Loop over each bunch crossing
  for ( int bc = cell->bcid_start(), i=0;  bc <= cell->bcid_end(); ++bc, ++i )
  {
    // Generate the pulse
    auto pulse = m_pulseGenerator->GenerateDeterministicPulse( rawEnergySamples[i], 0, bc*cell->bc_duration() );
    // Add gaussian noise
    // m_pulseGenerator[cell->layer()]->AddGaussianNoise(pulse);
    // Accumulate into pulse sum (Sum all pulses)
    for ( int j=0; j < pulse_size; ++j )
      pulse_sum[j] += (float)pulse->operator[](j);
  }

  // Add the pulse centered in the bunch crossing zero
  cell->setPulse( pulse_sum );
  return StatusCode::SUCCESS;
}



