
#include <cmath>
#include "Zee.h"
#include "helper.h"

using namespace generator;

Zee::Zee(const std::string name, IGenerator *gen):
  IAlgorithm(gen),
  IMsgService(name)
{
  declareProperty( "EtaMax"         , m_etaMax=1.4                );
  declareProperty( "MinPt"          , m_minPt=0.0                 );
}


Zee::~Zee()
{;}


StatusCode Zee::initialize()
{
  setMsgLevel(m_outputLevel);
  if(generator()->initialize().isFailure()){
    MSG_FATAL("Its not possible to initialize the generator. Abort!");
  }

  return StatusCode::SUCCESS;
}



StatusCode Zee::execute( generator::Event &ctx )
{

  HepMC3::GenEvent evt( HepMC3::Units::GEV, HepMC3::Units::MM);


  // Generate main event. Quit if too many failures.
  if (generator()->execute(evt).isFailure()){
    return StatusCode::FAILURE;
  }
  
  
  const auto main_event_t = sample_t();
  const auto main_event_z = sample_z();


  std::vector<const HepMC3::GenParticle*> zee; 

  for (auto part : evt.particles()) 
  {
    // Is electron and final state?
    if (part->abs_pid() == 11 && ParticleHelper::isFinal(part.get()) )
    {
      if(part->parents().empty()){
        continue;
      }

      auto mother = part->parents().at(0);
      // The mother is Z?
      if( mother->pid() == 23)
      {
        float eta = part->momentum().eta();
        float pt = part->momentum().pt();
        if ( std::abs(eta) < m_etaMax && pt > (m_minPt/1.e3) ){
          zee.push_back( part.get() );
        }
      }// From Z?
    }// Is electron?
  }

  if ( zee.empty() ){
    MSG_DEBUG( "There is not Zee event inside of this event");
    throw NotInterestingEvent();
  }

  MSG_INFO("Filling Zee events into the context...");

  // Each electron is a seed
  for ( auto& e : zee ){

    auto seed = generator::Seed( e->momentum().eta(), e->momentum().phi() );
    const auto vtx = e->production_vertex();

    seed.emplace_back( 1, 0, 
                       e->pid(), 
                       e->momentum().px(), 
                       e->momentum().py(), 
                       e->momentum().pz(), 
                       e->momentum().eta(), 
                       e->momentum().phi(), 
                       vtx->position().px(), 
                       vtx->position().py(), 
                       vtx->position().pz() + main_event_z, 
                       vtx->position().t() + main_event_t, 
                       e->momentum().e(), 
                       e->momentum().pt() ); 
    MSG_INFO("Add particle with PDGID " << e->pid() << " into the context.");
    ctx.push_back( seed );
  }
  

  ctx.setEventNumber(evt.event_number());
  return StatusCode::SUCCESS;
}



StatusCode Zee::finalize()
{
  MSG_INFO( "Finalize the Zee Event." );
  if(generator()->finalize().isFailure()){
    MSG_FATAL("Its not possible to finalize the generator. Abort!");
  }
  return StatusCode::SUCCESS;
}


