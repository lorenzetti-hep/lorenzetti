
#include "BoostedEvents.h"
#include "helper.h"
#include "TMath.h"


using namespace generator;



BoostedEvents::BoostedEvents(const std::string name, IGenerator *gen):
  IMsgService(name),
  SingleParticle(name,gen)
{
  declareProperty( "Energy"           , m_energy=-1                 );
  declareProperty( "Particle"         , m_pdgid=11                  );
  declareProperty( "HasLifetime"      , m_hasLifetime=false         );
  declareProperty( "AtRest"           , m_atRest=false              );
  declareProperty( "DeltaR"           , m_deltaR=0.2                );

}

BoostedEvents::~BoostedEvents()
{;}





StatusCode BoostedEvents::execute(  generator::Event &ctx )
{

  MSG_INFO( "Start boosted gun...");

  HepMC3::GenEvent evt( HepMC3::Units::GEV, HepMC3::Units::MM);

  generator()->clear();

  // For each seed inside of the event record we will
  // shoot a particle with DeltaR of distance of the
  // seed center. The energy of the particle will be a
  // fraction of the total energy of the seed.
  for ( auto& seed : *ctx )
  {
    float eta,phi=0.0;
    do{ // Generate eta/phi until the position is inside of the radio (deltaR)
      eta = seed.eta() +  (generator()->random_flat()-0.5); // can be positive or negative (-0.5->0.5) 
      phi = seed.phi() + (generator()->random_flat()-0.5); // can be positive or negative (-0.5->0.5)
    }while (dR( eta, phi, seed.eta(), seed.phi() ) > m_deltaR );

    // Generate the energy fraction desired to this seed
    float energy = seed.etot()*  (  0.1 + 1.8*generator()->random_flat() ); // Will get (0.1->1.9 percent) of the total energy (defined by Werner)

    // Fill the particle
    generator()->fill( m_pdgid, energy, eta, phi, m_atRest, m_hasLifetime);
  }
  
  
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
    if (ParticleHelper::isFinal(part.get())) {

      // Search for the associated seed into the event record
      for (auto& seed : *ctx)
      {
        // Get the associated seed for this generated particle
        // Add the particle into the seed particle list
        if (dR( part->momentum().eta(), part->momentum().phi(), 
                seed.eta(), seed.phi() ) <= m_deltaR )
        {
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

          MSG_INFO( "Adding particle: id = " << part->id() << 
                    " eta = " << part->momentum().eta() << 
                    " phi = " << part->momentum().phi() << 
                    " Et = "  << ParticleHelper::et(part.get()) );
        }
      }

    
    }// Add particle

  }// Loop over all generated particles

  return StatusCode::SUCCESS;
}


float BoostedEvents::dR( float eta1, float phi1, float eta2, float phi2 ) const
{
  float deta = fabs(eta1-eta2);
  float dphi = fabs(phi1-phi2)  < TMath::Pi() ? fabs(phi1-phi2) : 2*TMath::Pi() - fabs(phi1-phi2) ;
  return sqrt( deta*deta + dphi*dphi);
}



StatusCode BoostedEvents::initialize()
{
  return StatusCode::SUCCESS;
}

StatusCode BoostedEvents::finalize()
{
  return StatusCode::SUCCESS;
}


