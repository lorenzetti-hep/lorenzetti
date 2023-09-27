#include "OptimalFilter.h"
#include "CaloCell/CaloDetDescriptor.h"

using namespace Gaugi;


OptimalFilter::OptimalFilter( std::string name ) : 
  IMsgService(name),
  AlgTool()
{
  declareProperty( "WeightsEnergy"    , m_ofweightsEnergy={}  );
  declareProperty( "WeightsTime"      , m_ofweightsTime={}    );
  declareProperty( "OutputLevel"      , m_outputLevel=1       );
}

//!=====================================================================

OptimalFilter::~OptimalFilter()
{}

//!=====================================================================

StatusCode OptimalFilter::initialize()
{
  setMsgLevel(m_outputLevel);
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode OptimalFilter::finalize()
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode OptimalFilter::execute( SG::EventContext &/*ctx*/, Gaugi::EDM *edm ) const
{

  auto *cell = static_cast<xAOD::CaloDetDescriptor*>(edm);
  auto pulse = cell->pulse();
  float energy=0.0;
  float tau=0.0, EneTau=0.0;

  // Energy estimation
  if( m_ofweightsEnergy.size() != pulse.size() ){
    MSG_ERROR( "The ofweightsEnergy size its different than the pulse size." );
    return StatusCode::FAILURE;
  }else{
    for( unsigned sample=0; sample < pulse.size(); ++sample) 
      energy += pulse[sample]*m_ofweightsEnergy[sample];
  }

  // Time estimation
  if( m_ofweightsTime.size() != pulse.size() ){
    MSG_ERROR( "The ofweightsTime size its different than the pulse size." );
    return StatusCode::FAILURE;
  }
  else{
    for( unsigned sample=0; sample < pulse.size(); ++sample){
      EneTau += pulse[sample]*m_ofweightsTime[sample];
    }
    tau = EneTau/energy;
  }

  cell->setE(energy);
  cell->setTau(tau);
  return StatusCode::SUCCESS;
}


