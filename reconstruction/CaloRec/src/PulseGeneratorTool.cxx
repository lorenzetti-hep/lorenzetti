#include "PulseGeneratorTool.h"
#include "TPulseGenerator.h"

using namespace Gaugi;


PulseGeneratorTool::PulseGeneratorTool( std::string name ) : 
  ICaloCellTool( name ),
{
  declareProperty( "NSamples", m_nsamples , 7   );
  declareProperty( "CardPath", m_card     , ""  ) ;
}



PulseGeneratorTool::~PulseGeneratorTool()
{
  delete m_pulseGenerator;
}


StatusCode PulseGeneratorTool::initialize()
{

  m_pulseGenerator = new CPK::TPulseGenerator( m_nsamples, m_card.c_str());
  return StatusCode::SUCCESS;
}


StatusCode PulseGeneratorTool::finalize()
{
  return StatusCode::SUCCESS;
}


void PulseGeneratorTool::executeTool( xAOD::CaloCell *cell ) const
{
  auto pulse_size = m_pulseGenerator->GetPulseSize();
  
  // Get all energies for each bunch crossing 
  auto rawEnergySamples = cell->rawEnergySamples();
  
  // Create an pulse with zeros with n samples
  std::vector<float> pulse_sum(pulse_size, 0.0);
  // Loop over each bunch crossing
  for ( int bc = m_bc_id_start, i=0;  bc <= m_bc_id_end; ++bc, ++i )
  {
    // Generate the pulse
    auto pulse = m_pulseGenerator[cell->layer()]->GenerateDeterministicPulse( rawEnergySamples[i], 0, bc*m_bc_duration );
    // Add gaussian noise
    // m_pulseGenerator[cell->layer()]->AddGaussianNoise(pulse);
    // Accumulate into pulse sum (Sum all pulses)
    for ( unsigned j=0; j < pulse_size; ++j )
      pulse_sum[j] += (float)pulse->operator[](j);
  }

  // Add the pulse centered in the bunch crossing zero
  cell->setPulse( pulse_sum );
}



