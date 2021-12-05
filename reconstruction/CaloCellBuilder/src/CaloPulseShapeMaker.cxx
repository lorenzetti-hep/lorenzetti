#include "CaloCell/CaloDetDescriptor.h"
#include "CaloPulseShapeMaker.h"

using namespace Gaugi;


CaloPulseShapeMaker::CaloPulseShapeMaker( std::string name ) : 
  IMsgService(name),
  AlgTool(),
  m_origin(0),
  m_resolution(0)
{
  declareProperty( "ShaperFile"  , m_shaperFile="" );
  declareProperty( "OutputLevel" , m_outputLevel=1 );
}

//!=====================================================================

CaloPulseShapeMaker::~CaloPulseShapeMaker()
{}

//!=====================================================================

StatusCode CaloPulseShapeMaker::initialize()
{
  setMsgLevel( (MSG::Level)m_outputLevel );
  MSG_DEBUG( "Initializing CaloPulseShapeMaker");
  MSG_DEBUG( "Reading shaper values from: " << m_shaperFile);
  ReadShaper( m_shaperFile );
  MSG_DEBUG( "Shaper read successfully");
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloPulseShapeMaker::finalize()
{
  m_pulseShape.clear();
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloPulseShapeMaker::execute( const xAOD::EventInfo * /*evt*/, Gaugi::EDM *edm ) const
{
  auto *cell = static_cast<xAOD::CaloDetDescriptor*>(edm);
  cell->setPulseShape( m_pulseShape, m_origin, m_resolution );
  return StatusCode::SUCCESS;
}

//!=====================================================================

void CaloPulseShapeMaker::ReadShaper( std::string filepath )
{
  m_pulseShape.clear();

  /*! open file */
  std::ifstream file;
  file.open(filepath);

  if (file) {
     unsigned int i = 0;
     float time, sample;
     while (file >> time >> sample) // loop on the input operation, not eof
     {
        m_pulseShape.push_back(std::make_pair(time, sample));
        if (time == 0.0) m_origin = i;
        i++;
     }
  } else {
    MSG_FATAL( "Invalid shaper path: " << filepath );
  }

  /*! close file */
  file.close();

  /*! calculate shaper resolution */
  m_resolution = m_pulseShape.size() > 2 ?
      m_pulseShape[1].first - m_pulseShape[0].first :
      m_pulseShape[0].first;
}
