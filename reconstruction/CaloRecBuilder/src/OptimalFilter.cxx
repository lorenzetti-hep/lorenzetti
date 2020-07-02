#include "OptimalFilter.h"

using namespace Gaugi;


OptimalFilter::OptimalFilter( std::string name ) : 
  IMsgService(name),
  CaloTool()
{
  declareProperty( "Weights"    , m_ofweights={}  );
  declareProperty( "OutputLevel", m_outputLevel=1 );
}



OptimalFilter::~OptimalFilter()
{}


StatusCode OptimalFilter::initialize()
{
  setMsgLevel(m_outputLevel);
  return StatusCode::SUCCESS;
}


StatusCode OptimalFilter::finalize()
{
  return StatusCode::SUCCESS;
}



StatusCode OptimalFilter::executeTool( const xAOD::EventInfo * /*evt*/, xAOD::RawCell *cell ) const
{
	auto pulse = cell->pulse();
	float energy=0.0;

  //float avgmu = evt->avgmu();

	if( m_ofweights.size() != pulse.size() ){
		MSG_ERROR( "The ofweights size its different than the pulse size." );
		return StatusCode::FAILURE;
	}else{
		for( unsigned sample=0; sample < pulse.size(); ++sample) 
			energy += pulse[sample]*m_ofweights[sample];
	}

	cell->setEnergy(energy);
	return StatusCode::SUCCESS;
}


// Just for python import in ROOT
StatusCode OptimalFilter::executeTool( const xAOD::EventInfo *, xAOD::CaloCell * ) const {return StatusCode::SUCCESS;}
StatusCode OptimalFilter::executeTool( const xAOD::EventInfo *, xAOD::CaloCluster * ) const {return StatusCode::SUCCESS;}
StatusCode OptimalFilter::executeTool( const xAOD::EventInfo *, xAOD::TruthParticle * ) const {return StatusCode::SUCCESS;}


