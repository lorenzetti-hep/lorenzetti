
#include "SingleParticle.h"
#include "helper.h"
#include <cmath>

using namespace generator;


SingleParticle::SingleParticle(const std::string name, IGenerator *gen): 
  IMsgService(name),
  IAlgorithm(gen)
{
  declareProperty( "Eta"              , m_eta=0.0                   );
  declareProperty( "Phi"              , m_phi=0.0                   );
  declareProperty( "Energy"           , m_energy=-1                 );
  declareProperty( "EnergyMin"        , m_energyMin=0               );
  declareProperty( "EnergyMax"        , m_energyMax=100*1e3         );
  declareProperty( "Particle"         , m_pdgid=11                  );
  declareProperty( "HasLifetime"      , m_hasLifetime=false         );
  declareProperty( "AtRest"           , m_atRest=false              );
  declareProperty( "DoRangedEta"      , m_doRangedEta=false         );
  declareProperty( "EtaMin"           , m_etaMin=-2.5               );
  declareProperty( "EtaMax"           , m_etaMax=2.5                );
}



StatusCode SingleParticle::initialize()
{
  MSG_INFO( "Initializing the SingleParticle..." );
  setMsgLevel(m_outputLevel);
  if(generator()->initialize().isFailure()){
    MSG_FATAL("Its not possible to initialize the generator. Abort!");
  }

  return StatusCode::SUCCESS;
}



StatusCode SingleParticle::execute( generator::Event &ctx )
{

  MSG_INFO( "Start particle gun...");


  // Fix energy or flat distribution between min->max 
  float energy = m_energy>0.0 ? m_energy : ( m_energyMin + (m_energyMax-m_energyMin)*generator()->random_flat());
  float eta = m_doRangedEta? ( m_etaMin + (m_etaMax-m_etaMin)*generator()->random_flat()) : m_eta;

  generator()->clear();

  MSG_INFO("Fill particle: eta = " << eta << " phi = " << m_phi << " energy = " << energy);
  generator()->fill( m_pdgid, energy/1e3, eta, m_phi, m_atRest, m_hasLifetime);
  
  
  MSG_INFO("Generate particles...");
  HepMC3::GenEvent evt( HepMC3::Units::GEV, HepMC3::Units::MM);

  // Generate main event. Quit if too many failures.
  if (generator()->execute(evt).isFailure()){
    return StatusCode::FAILURE;
  }


  for (auto part : evt.particles()) 
  {
    // Find any unrecognized particle codes.
    int id = part->pid();

    if (id == 0)
    {
      MSG_WARNING("Unknown code id = " << id );
      continue;
    }

    const auto main_event_t = sample_t();
    const auto main_event_z = sample_z();
    
    // Study final-state particles.
    if (ParticleHelper::isFinal(part.get())) 
    {
      auto seed = Seed( part->momentum().eta(), part->momentum().phi() );
      const auto vtx = part->production_vertex();
      seed.emplace_back( 1, 0, 
                   part->pid(), 
                   part->momentum().px(), 
                   part->momentum().py(), 
                   part->momentum().pz(), 
                   part->momentum().eta(), 
                   part->momentum().phi(), 
                   vtx->position().px(), 
                   vtx->position().py(), 
                   vtx->position().pz() + main_event_z, 
                   vtx->position().t()  + main_event_t, 
                   part->momentum().e(), 
                   part->momentum().pt() ); 


      ctx.push_back( seed );

      MSG_INFO( "Adding particle: id = " << part->id() << 
                " eta = " << part->momentum().eta() << 
                " phi = " << part->momentum().phi() << 
                " Et  = " << ParticleHelper::et(part.get()) );

    }// Add particle

  }// Loop over all generated particles

  return StatusCode::SUCCESS;
}



StatusCode SingleParticle::finalize()
{
  MSG_INFO( "Finalize the SingleParticle Event." );
  if(generator()->finalize().isFailure()){
    MSG_FATAL("Its not possible to finalize the generator. Abort!");
  }
  return StatusCode::SUCCESS;
}
