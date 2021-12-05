#include <sstream>
#include <TPrincipal.h>
#include <TMatrixD.h>
#include <TVectorD.h>
#include "OptimalFilter.h"

using namespace Gaugi;


OptimalFilter::OptimalFilter( std::string name ) : 
  IMsgService(name),
  AlgTool()
{
  declareProperty( "UseDoubleRestriction",   m_useDoubleRestriction=true  );
  declareProperty( "SamplingRate",           m_samplingRate=25 );
  declareProperty( "StartSamplingBC",        m_startSamplingBC=-3 );
  declareProperty( "EnableCalibration",      m_doCalibration=false );
  declareProperty( "CalibrationDatasetFile", m_calibrationDatasetFile="" );
  declareProperty( "StaticWeights",          m_staticCoeffs={} );
  declareProperty( "OutputLevel",            m_outputLevel=1 );
}

//!=====================================================================

OptimalFilter::~OptimalFilter()
{}

//!=====================================================================

StatusCode OptimalFilter::initialize()
{
  setMsgLevel(m_outputLevel);
  MSG_DEBUG( "Initializing OptimalFilter");
  if (m_doCalibration) {
    MSG_DEBUG( "Reading calibration dataset from: " << m_calibrationDatasetFile);
    ReadCalibrationDataset();
  }
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode OptimalFilter::finalize()
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode OptimalFilter::execute( const xAOD::EventInfo * /*evt*/, Gaugi::EDM *edm ) const
{
  MSG_DEBUG( "execute()" );

  auto *cell = static_cast<xAOD::CaloDetDescriptor*>(edm);
  auto pulse = cell->pulse();

  if (m_outputLevel <= MSG::DEBUG) {
    std::ostringstream log;
    log<<"pulse =";
    for (unsigned int i = 0; i < pulse.size(); ++i) log<<" "<<pulse[i];
    MSG_DEBUG( log.str() );
  }

  auto coeffs = m_staticCoeffs.size() ? m_staticCoeffs : CalculateOptCoeffs(pulse.size(), cell);

  if (m_outputLevel <= MSG::DEBUG) {
    std::ostringstream log;
    log<<"coeffs =";
    for (unsigned int i = 0; i < coeffs.size(); ++i) log<<" "<<coeffs[i];
    MSG_DEBUG( log.str() );
  }

  float energy = Filter(pulse, coeffs);
  cell->setE(energy);
  MSG_DEBUG( "energy = "<<energy );

  return StatusCode::SUCCESS;
}

float OptimalFilter::Filter(const std::vector<float> &pulse, const std::vector<float> &coeffs) const
{
  double energy = 0;
  for (unsigned int i = 0; i < pulse.size(); ++i) {
    energy += (pulse[i] * coeffs[i]);
  }
  return energy;
}

const std::vector<float> OptimalFilter::CalculateOptCoeffs( const unsigned int filterSize, const xAOD::CaloDetDescriptor* cell) const
{
  const std::vector<std::pair<float,float>> pulseShape = cell->pulseShape();
  const unsigned int shaperOriginIndex = cell->pulseShapeOrigin();
  const float shaperResolution = cell->pulseShapeResolution();

  double g[filterSize];
  double dg[filterSize];
  double dx;

  for (unsigned int i = 0; i < filterSize; i++) {
    int shaperIndex = int(shaperOriginIndex) + (i + m_startSamplingBC) * (m_samplingRate / shaperResolution);
    if (shaperIndex < 0) shaperIndex = 0;
    if (shaperIndex > pulseShape.size()) shaperIndex = pulseShape.size() -1;

    g[i] = pulseShape[shaperIndex].second;

    dx = pulseShape[shaperIndex].first - pulseShape[shaperIndex-1].first;
    dg[i] = (pulseShape[shaperIndex].second - pulseShape[shaperIndex-1].second) / dx;
  }

  // unitary covariance matrix
  const TMatrixD *C = GetNoiseCovarianceMatrix(filterSize);

  unsigned int systemDimension = filterSize + 2;
  if (m_useDoubleRestriction) systemDimension++;

  // defines solution vector
  TVectorD b(systemDimension);
  b.Zero();
  b[filterSize] = 1;

  // defines matrix A, where Aw=b
  TMatrixD A(systemDimension, systemDimension);
  A.Zero();

  for (unsigned int i = 0; i < filterSize; i++) {
    // copies C to A
    for (unsigned int j = 0; j < filterSize; j++) {
      A[i][j] = (*C)[i][j];
    }

    // copies g to A
    A[filterSize][i] = g[i];
    A[i][filterSize] = -g[i];

    // copries dg to A
    A[filterSize + 1][i] = dg[i];
    A[i][filterSize + 1] = -dg[i];

    // sets unitary column
    if (m_useDoubleRestriction) {
      A[filterSize + 2][i] = 1;
      A[i][filterSize + 2] = -1;
    }
  }

  // defines the linear system solution -> w = inv(A)*b
  A.Invert();
  TVectorD w = A * b;

  // gets the weights
  std::vector<float> coeffs;
  for (unsigned int i = 0; i < filterSize; i++) {
    coeffs.push_back((float) w[i]);
  }

  delete C;

  return coeffs;
}

const TMatrixD* OptimalFilter::GetNoiseCovarianceMatrix( unsigned int filterSize ) const
{
  if (!m_doCalibration) {
    MSG_DEBUG( "Using unitary covariance matrix" );
    TMatrixD* unitMatrix = new TMatrixD(filterSize, filterSize);
    unitMatrix->UnitMatrix();
    return unitMatrix;
  }

  MSG_DEBUG( "Using noise covariance matrix" );

  if (m_calibrationDataset->GetNcols() != (int) filterSize) {
    MSG_FATAL( "Invalid noise dataset" );
    throw std::invalid_argument("Invalid noise dataset");
  }

  TPrincipal *principal = new TPrincipal(filterSize);
  for (int i = 0; i < m_calibrationDataset->GetNrows(); i++) {
    const TMatrixDRow_const row = (*m_calibrationDataset)[i];
    principal->AddRow(row.GetPtr());
  }

  TMatrixD * covarianceMatrix = new TMatrixD(*principal->GetCovarianceMatrix());

  // transforms the symetric matrix
  for (unsigned int i = 0; i < filterSize; ++i)
    for (unsigned int j = i; j < filterSize; ++j)
      (*covarianceMatrix)[i][j] = (*covarianceMatrix)[j][i];

  return covarianceMatrix;
}

StatusCode OptimalFilter::ReadCalibrationDataset()
{
  std::vector<std::vector<double>> m;

  std::ifstream file;
  std::string   line;

  file.open(m_calibrationDatasetFile.c_str());
  if (!file.is_open()) {
    MSG_FATAL( "Invalid shaper path: " << m_calibrationDatasetFile );
    return StatusCode::FAILURE;
  }

  while (!std::getline(file, line, '\n').eof()) {
    // skip empty or comment lines
    if (line.size() == 0 || line[0] == '#')
      continue;

    std::istringstream reader(line);
    std::vector<double> lineData;

    while (!reader.eof()) {
      double val;
      reader >> val;

      if (reader.fail()) break;

      lineData.push_back(val);
    }

    m.push_back(lineData);
  }

  file.close();

  int rows = m.size();
  int cols = m[0].size();
  m_calibrationDataset = new TMatrixD(rows, cols);

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      (*m_calibrationDataset)[i][j] = m[i][j];
    }
    m[i].clear();
  }
  m.clear();

  return StatusCode::SUCCESS;
}
