#include "ConstrainedOptimalFilter.h"
#include "CaloCell/CaloDetDescriptor.h"

using namespace Gaugi;


ConstrainedOptimalFilter::ConstrainedOptimalFilter( std::string name ) : 
  IMsgService(name),
  AlgTool()
{
  declareProperty( "PulsePath"        , m_pulsepath       );
  declareProperty( "Threshold"        , m_threshold=0.0   );
  declareProperty( "NSamples"         , m_nsamples=0      );
  declareProperty( "StartSamplingBC"  , m_startSamplingBC );
  declareProperty( "SamplingRate"     , m_samplingRate=25 ); 
  declareProperty( "OutputLevel"      , m_outputLevel=1   );
}

void ConstrainedOptimalFilter::ReadShaper( std::string filepath )
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

ConstrainedOptimalFilter::~ConstrainedOptimalFilter()
{}

//!=====================================================================

StatusCode ConstrainedOptimalFilter::initialize()
{
  setMsgLevel(m_outputLevel);
  ReadShaper(m_pulsepath);
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode ConstrainedOptimalFilter::finalize()
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode ConstrainedOptimalFilter::execute( SG::EventContext &/*ctx*/, Gaugi::EDM *edm ) const
{
  std::vector<float> refpulse;
  GeneratePulse(refpulse);
  TMatrixD H(m_nsamples, m_nsamples);
  TMatrixD saveH(m_nsamples, m_nsamples);
  int extraZeros = int(m_nsamples / 2);
  std::vector<float> fullVector;
  for (int i = 0; i < extraZeros; i++) fullVector.push_back(0.0);
  for (auto element : refpulse) fullVector.push_back(element);
  for (int i = 0; i < extraZeros; i++) fullVector.push_back(0.0);
  
  for (int line = 0; line < m_nsamples; line++){
    int firstIndex = fullVector.size() / 2 - line;
    for (int col = 0; col < m_nsamples; col++){
      H[line][col] = fullVector[firstIndex + col];
      saveH[line][col] = fullVector[firstIndex + col];
    }
  }

  auto *cell = static_cast<xAOD::CaloDetDescriptor*>(edm); 
  auto pulse = cell->pulse();
  TVectorD tpulse(m_nsamples);
  for (int i = 0; i< m_nsamples; i++) tpulse[i] = pulse[i];
  auto invH =  H.T().Invert();
  auto a_hat = invH*tpulse;
  
  std::vector<bool> passedSamples(m_nsamples); 
  int totalSamples = 0;
  int newCentralSample = 0;
  for (int i = 0; i<m_nsamples; i++){
    if (a_hat[i] >= m_threshold){
      passedSamples[i] = true;
      totalSamples++;
      if (i < m_nsamples/2) newCentralSample++;
    }
    else passedSamples[i] = false;
  } 
  if (!passedSamples[m_nsamples/2]){
    passedSamples[m_nsamples/2] = true; //always accept the central samples
    totalSamples++;
  }

  TMatrixD G(totalSamples, m_nsamples);
  TMatrixD saveG(totalSamples, m_nsamples);
  int k = 0;
  for (int i = 0; i<m_nsamples; i++){
    if (passedSamples[i]){
      for (int j = 0; j < m_nsamples; j++) {
        G[k][j]= saveH[i][j];
        saveG[k][j] = saveH[i][j];
      }
      k++;
    }
  }
  auto a_hat_hat = ((saveG*G.T()).Invert())*saveG*tpulse;
  cell->setE(a_hat_hat[newCentralSample]);
  return StatusCode::SUCCESS;
}

void ConstrainedOptimalFilter::GeneratePulse(  std::vector<float> &pulse) const
{
  pulse.resize( m_nsamples );
  float shr    = m_shaperResolution;  
  float shzi   = m_shaperZeroIndex; 

  for (int i = 0; i < m_nsamples; i++) {
    int shaperIndex = int(shzi) + (i + m_startSamplingBC) * (m_samplingRate / shr);
    if (shaperIndex < 0 || shaperIndex > (int)m_shaper.size() - 1){
      pulse[i] = 0;
      continue;
    }
    pulse[i] += m_shaper[shaperIndex];
  }
}
