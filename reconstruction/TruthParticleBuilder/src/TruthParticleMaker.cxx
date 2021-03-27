
#include "G4Kernel/CaloPhiRange.h"
#include "TruthParticle/TruthParticle.h"
#include "TruthParticleMaker.h"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "TH1F.h"
#include "TH2F.h"

using namespace SG;
using namespace Gaugi;



TruthParticleMaker::TruthParticleMaker( std::string name ) : 
  IMsgService(name),
  Algorithm()
{
  // Key inputs
  declareProperty( "EventKey"       , m_eventKey="EventInfo"            );
  // Key outputs
  declareProperty( "TruthKey"       , m_truthKey="Particles"            );
  // Algorithm configuration
  declareProperty( "OutputLevel"    , m_outputLevel=1                   );
  declareProperty( "HistogramPath"  , m_histPath="Clusters"             );
}



TruthParticleMaker::~TruthParticleMaker()
{;}


StatusCode TruthParticleMaker::initialize()
{
  setMsgLevel(m_outputLevel);
  return StatusCode::SUCCESS;
}


StatusCode TruthParticleMaker::bookHistograms( SG::EventContext &ctx ) const
{
  auto store = ctx.getStoreGateSvc();

  store->mkdir( m_histPath );  
  return StatusCode::SUCCESS;
}


StatusCode TruthParticleMaker::pre_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}


StatusCode TruthParticleMaker::execute( EventContext &/*ctx*/, const G4Step * /*step*/ ) const
{
  return StatusCode::SUCCESS;
}


StatusCode TruthParticleMaker::finalize()
{
  return StatusCode::SUCCESS;
}


StatusCode TruthParticleMaker::post_execute( EventContext &ctx ) const
{

  SG::WriteHandle<xAOD::TruthParticleContainer> particles(m_truthKey, ctx);

  particles.record( std::unique_ptr<xAOD::TruthParticleContainer>(new xAOD::TruthParticleContainer()) );
  
  // Event info
  SG::ReadHandle<xAOD::EventInfoContainer> event(m_eventKey, ctx);

  if( !event.isValid() ){
    MSG_FATAL( "It's not possible to read the xAOD::EventInfoContainer from this Context" );
  }

  auto *evt = (**event.ptr()).front();

  for ( auto& seed : evt->allSeeds() )
  {

    xAOD::TruthParticle *particle = new xAOD::TruthParticle();
    particle->setEt( seed.et );
    particle->setEta( seed.eta );
    particle->setPhi( seed.phi );
    particle->setPdgid( seed.pdgid );
    particles->push_back( particle );
  }

  MSG_INFO( "We found " << particles->size() << " particles (seeds) inside of this event." );

  return StatusCode::SUCCESS;
}





StatusCode TruthParticleMaker::fillHistograms(EventContext &ctx ) const
{

  MSG_DEBUG( "Fill all histograms" );
  
  auto store = ctx.getStoreGateSvc();

  SG::ReadHandle<xAOD::TruthParticleContainer> particles( m_truthKey, ctx );

  if( !particles.isValid() ){
    MSG_ERROR("It's not possible to get the TruthParticleContainer inside of this Context using this key: " << m_truthKey);
    return StatusCode::FAILURE;
  }

  MSG_DEBUG( "We found " << particles->size() << " particles (seeds) inside of this event." );

  store->cd(m_histPath);



  for( const auto& particle : **particles.ptr() ){
  

    if ( !particle->caloCluster() ) continue;
    MSG_DEBUG("============== Particle Information ==============");
    
    MSG_DEBUG( "Et       : " << particle->et() );
    MSG_DEBUG( "Eta      : " << particle->eta() );
    MSG_DEBUG( "Phi      : " << particle->phi() );
    
    MSG_DEBUG("=================================================");
  }

  return StatusCode::SUCCESS;
}





