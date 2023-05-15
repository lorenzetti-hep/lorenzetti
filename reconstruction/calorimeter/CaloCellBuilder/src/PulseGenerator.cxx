
#include "CaloCell/CaloDetDescriptor.h"
#include "PulseGenerator.h"
#include "Randomize.hh"
#include "TRandom.h"

using namespace Gaugi;


PulseGenerator::PulseGenerator( std::string name ) : 
  IMsgService(name),
  AlgTool(),
  m_shaperZeroIndex(0),
  m_shaperResolution(0),
  m_rng(0)
{
  declareProperty( "ShaperFile"       , m_shaperFile=""         );
  declareProperty( "Pedestal"         , m_pedestal = 0          );
  declareProperty( "DeformationMean"  , m_deformationMean=0     );
  declareProperty( "DeformationStd"   , m_deformationStd=0      );
  declareProperty( "SamplingRate"     , m_samplingRate=25       );  
  declareProperty( "OutputLevel"      , m_outputLevel=1         );
  declareProperty( "NoiseMean"        , m_noiseMean=0           );
  declareProperty( "NoiseStd"         , m_noiseStd=0            );
  declareProperty( "NSamples"         , m_nsamples=7            );
  declareProperty( "StartSamplingBC"  , m_startSamplingBC=0     );
}

//!=====================================================================

PulseGenerator::~PulseGenerator()
{;}

//!=====================================================================

StatusCode PulseGenerator::initialize()
{
  setMsgLevel( (MSG::Level)m_outputLevel );
  MSG_DEBUG( "Reading shaper values from: " << m_shaperFile << " and " << m_nsamples << " samples.");
  ReadShaper( m_shaperFile );
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode PulseGenerator::finalize()
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode PulseGenerator::execute( SG::EventContext &/*ctx*/, Gaugi::EDM *edm ) const
{
  auto *cell = static_cast<xAOD::CaloDetDescriptor*>(edm);

  auto pulse_size = m_nsamples;

  // Create an pulse with zeros with n samples
  std::vector<float> pulse_sum(pulse_size, 0.0);
  // Loop over each bunch crossing
  for ( int bcid = cell->bcid_start();  bcid <= cell->bcid_end(); ++bcid )
  {
    // Generate the pulse
    std::vector<float> pulse;
    GenerateDeterministicPulse( pulse, cell->edep(bcid), 0, bcid*cell->bc_duration() );
    // Accumulate into pulse sum (Sum all pulses)
    for ( int samp=0; samp < pulse_size; ++samp ){
      pulse_sum[samp] += pulse[samp];
    }

    cell->setPulse( bcid, pulse ); 
  }

  // Add gaussian noise
  AddGaussianNoise(pulse_sum, m_noiseMean, m_noiseStd);


  // Add the integrated pulse centered in the bunch crossing zero
  cell->setPulse( pulse_sum );


  return StatusCode::SUCCESS;
}

//!=====================================================================

void PulseGenerator::ReadShaper( std::string filepath )
{
  std::ifstream file;
  m_timeSeries.clear();
  m_shaper.clear();
  file.open(filepath);
  if (file) {
     int i=0;
     float a, b;
     while (file >> a >> b) // loop on the input operation, not eof
     {
        m_timeSeries.push_back(a);
        m_shaper.push_back(b);
        if (a == 0.0) m_shaperZeroIndex = i;
        i++;
     }
  } else {
    MSG_FATAL( "Invalid shaper path: " << filepath );
  }
  file.close();
  m_shaperResolution = m_shaper.size() > 2 ? m_timeSeries[1] - m_timeSeries[0] : m_timeSeries[0];
}

//!=====================================================================

void PulseGenerator::AddGaussianNoise( std::vector<float> &pulse, float noiseMean, float noiseStd) const
{
  for ( auto &value : pulse )
    value += m_rng.Gaus( noiseMean, noiseStd );
}

//!=====================================================================

void PulseGenerator::GenerateDeterministicPulse(  std::vector<float> &pulse,  float amplitude, float phase, float lag) const
{
  pulse.resize( m_nsamples );
  float shr    = m_shaperResolution;  
  float shzi   = m_shaperZeroIndex; 

  for (int i = 0; i < m_nsamples; i++) {
    // random deformation (normal from geant)
    float deformation = m_rng.Gaus(m_deformationMean, m_deformationStd);
    int shaperIndex = int(shzi) - int(lag / shr) + (i + m_startSamplingBC) * (m_samplingRate / shr) + round(phase / shr);
    if (shaperIndex < 0 || shaperIndex > (int)m_shaper.size() - 1){
      pulse[i] = 0;
      continue;
    }
    pulse[i] += amplitude * m_shaper[shaperIndex] + m_pedestal + deformation;
  }
}






