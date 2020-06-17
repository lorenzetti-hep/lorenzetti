
#include "BoostedEvents.h"
#include "Pythia8/Pythia.h"
#include "TMath.h"

using namespace Pythia8;
using namespace generator;


BoostedEvents::BoostedEvents(): 
  IMsgService("BoostedEvents"), 
  ParticleGun()
{
  declareProperty( "Energy"           , m_energy=-1                 );
  declareProperty( "Particle"         , m_pdgid=11                  );
  declareProperty( "HasLifetime"      , m_hasLifetime=false         );
  declareProperty( "AtRest"           , m_atRest=false              );
  declareProperty( "DeltaR"           , m_deltaR=0.2                );

}

BoostedEvents::~BoostedEvents()
{;}



StatusCode BoostedEvents::initialize()
{
  MSG_INFO( "Initializing the BoostedEvents..." );
  std::stringstream cmdseed; cmdseed << "Random:seed = " << m_seed;
  
  // Read in commands from external file.
  m_generator.readString("ProcessLevel:all = off");
  m_generator.readString("Random:setSeed = on");
  m_generator.readString(cmdseed.str());
  // Key requirement: switch off ProcessLevel, and thereby also PartonLevel.

  // Switch off automatic event listing in favour of manual.
  m_generator.readString("Next:numberShowInfo = 1");
  m_generator.readString("Next:numberShowProcess = 1");
  m_generator.readString("Next:numberShowEvent = 1");

  //m_nAbort = m_generator.mode("Main:timesAllowErrors");
  // Initialization for main (LHC) event
  m_generator.init();

  return StatusCode::SUCCESS;
}



StatusCode BoostedEvents::execute( generator::Event &event )
{

  MSG_INFO( "Start boosted gun...");

  clearParticles();

  // For each seed inside of the event record we will
  // shoot a particle with DeltaR of distance of the
  // seed center. The energy of the particle will be a
  // fraction of the total energy of the seed.
  for ( auto& seed : *event )
  {
    float eta,phi=0.0;
    do{ // Generate eta/phi until the position is inside of the radio (deltaR)
      eta = seed.eta() +  (m_generator.rndm.flat()-0.5); // can be positive or negative (-0.5->0.5) 
      phi = seed.phi() + (m_generator.rndm.flat()-0.5); // can be positive or negative (-0.5->0.5)
    }while (dR( eta, phi, seed.eta(), seed.phi() ) > m_deltaR );

    // Generate the energy fraction desired to this seed
    float energy = seed.etot()*  (  0.1 + 1.8*m_generator.rndm.flat() ); // Will get (0.1->1.9 percent) of the total energy (defined by Werner)

    // Fill the particle
    fillParticle( m_pdgid, energy, eta, phi, m_atRest, m_hasLifetime);
  }
  
  
  if (!m_generator.next()) {
    if (m_iAbort++>m_nAbort){
      MSG_ERROR("Event generation aborted prematurely, owing to error in main event!" );
      throw AbortPrematurely();
    }
  }

  float weight = m_generator.info.mergingWeight();
  float evtweight = m_generator.info.weight();
  weight *= evtweight;

  // Do not print zero-weight events.
  if ( weight == 0. ) {
    MSG_WARNING("Pythia generation return weight zero.");
    return StatusCode::FAILURE;
  }


  // The number of particles will be equal the number of seed always.
  for ( int i=0; i<m_generator.event.size(); ++i ){

    auto *p = &m_generator.event[i];
    auto *pdt = &m_generator.particleData;

    // Find any unrecognized particle codes.
    int id = p->id();

    if (id == 0 || !pdt->isParticle(id)){
      MSG_WARNING("Unknown code id = " << id );
      continue;
    }



    // Study final-state particles.
    if (p->isFinal()) {

      // Search for the associated seed into the event record
      for (auto& seed : *event){
        // Get the associated seed for this generated particle
        // Add the particle into the seed particle list
        if (dR( p->eta(), p->phi(), seed.eta(), seed.phi() ) <= m_deltaR ){
          seed.emplace_back( 1, 0, p->id(), p->px(), p->py(), p->pz(), p->eta(), p->phi(), 
                             p->xProd(), p->yProd(), p->zProd(), p->tProd(), 
                             p->e(), p->eT() ); 
          MSG_INFO( "Adding particle: id = " << p->id() << " eta = " << p->eta() << " phi = " << p->phi() << " Et = " << p->eT() );
        }
      }

    
    }// Add particle

  }// Loop over all generated particles

  return StatusCode::SUCCESS;
}



StatusCode BoostedEvents::finalize()
{
  MSG_INFO( "Finalize the BoostedEvents Event." );
  m_generator.stat();
  return StatusCode::SUCCESS;
}



float BoostedEvents::dR( float eta1, float phi1, float eta2, float phi2 ) const
{
  float deta = fabs(eta1-eta2);
  float dphi = fabs(phi1-phi2)  < TMath::Pi() ? fabs(phi1-phi2) : 2*TMath::Pi() - fabs(phi1-phi2) ;
  return sqrt( deta*deta + dphi*dphi);
}



