#include "PulseGenerator.h"
#include "TPulseGenerator.h"

using namespace Gaugi;


PulseGenerator::PulseGenerator( std::string name ) : 
  IMsgService(name),
  CaloTool()
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
  MSG_INFO( "Reading shaper values from: " << m_shaperFile );
  setMsgLevel( (MSG::Level)m_outputLevel );
  m_pulseGenerator = new CPK::TPulseGenerator( m_nsamples, m_shaperFile.c_str());
  return StatusCode::SUCCESS;
}


StatusCode PulseGenerator::finalize()
{
  return StatusCode::SUCCESS;
}


StatusCode PulseGenerator::executeTool( xAOD::RawCell *cell ) const
{
  auto pulse_size = m_pulseGenerator->GetPulseSize();
  
  // Get all energies for each bunch crossing 
  auto rawEnergySamples = cell->rawEnergySamples();

  //std::cout << "RawEnergySamples = ";
  //for (auto v : rawEnergySamples )
  //  std::cout << v << " ";
  //std::cout << std::endl;

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
    delete pulse; // This must be deleted to avoid memory leak since spk uses "new" internally
  }

  // Add the pulse centered in the bunch crossing zero
  cell->setPulse( pulse_sum );
  return StatusCode::SUCCESS;
}



// Just for python import in ROOT
StatusCode PulseGenerator::executeTool( xAOD::CaloCell * ) const {return StatusCode::SUCCESS;}
StatusCode PulseGenerator::executeTool( xAOD::CaloCluster * ) const {return StatusCode::SUCCESS;}
StatusCode PulseGenerator::executeTool( xAOD::TruthParticle * ) const {return StatusCode::SUCCESS;}










