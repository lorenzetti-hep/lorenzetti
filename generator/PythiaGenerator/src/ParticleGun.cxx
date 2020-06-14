
#include "ParticleGun.h"
#include "Pythia8/Pythia.h"


using namespace Pythia8;
using namespace generator;


ParticleGun::ParticleGun(): 
  IMsgService("ParticleGun"), 
  Algorithm()
{
  declareProperty( "Eta"              , m_eta=0.0                   );
  declareProperty( "Phi"              , m_phi=0.0                   );
  declareProperty( "Energy"           , m_energy=-1                 );
  declareProperty( "EnergyMin"        , m_energyMin=-1              );
  declareProperty( "EnergyMax"        , m_energyMax=-1              );
  declareProperty( "Particle"         , m_pdgid=11                  );
  declareProperty( "NumberOfParticles", m_numberOfParticles=1       );
  declareProperty( "HasLifetime"      , m_hasLifetime=false         );
  declareProperty( "AtRest"           , m_atRest=false              );
}

ParticleGun::~ParticleGun()
{;}



StatusCode ParticleGun::initialize()
{
  MSG_INFO( "Initializing the ParticleGun..." );
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
  
  m_theta = acos( tanh( m_eta ) );


  // Options to energy distribution
  if(m_energy!=-1){
    m_energyMin=m_energyMax=m_energy;
  }

  return StatusCode::SUCCESS;
}



StatusCode ParticleGun::execute( generator::Event &event )
{

  MSG_INFO( "Start particle gun...");

  ParticleData& pdt = m_generator.particleData;


  fillParticle( m_generator.event, m_pdgid, m_energyMin/1e3, m_energyMax/1e3, m_theta, m_phi, pdt, m_atRest, m_hasLifetime);
  
  
  MSG_INFO("next...");
  // Generate main event. Quit if too many failures.
  m_generator.next();
  
  if (!m_generator.next()) {
    if (m_iAbort++>m_nAbort){
      MSG_ERROR("Event generation aborted prematurely, owing to error in main event!" );
      throw AbortPrematurely();
    }
  }

  MSG_INFO("next...");
  double weight = m_generator.info.mergingWeight();
  double evtweight = m_generator.info.weight();
  weight *= evtweight;

  // Do not print zero-weight events.
  if ( weight == 0. ) {
    MSG_WARNING("Pythia generation return weight zero.");
    return StatusCode::FAILURE;
  }


  MSG_INFO("next...");
  for ( int i=0; i<m_generator.event.size(); ++i ){

    MSG_INFO("loop in " << i);
    auto *p = &m_generator.event[i];
    // Find any unrecognized particle codes.
    int id = p->id();

    if (id == 0 || !pdt.isParticle(id)){
      MSG_WARNING(" Error! Unknown code id = " << id );
      continue;
    }


    // Study final-state particles.
    if (p->isFinal()) {

      MSG_INFO("loop in " << i << " Add!");
    
      auto seed = generator::Seed( p->eta(), p->phi() );

      seed.emplace_back( 1, 0, p->id(), p->px(), p->py(), p->pz(), p->eta(), p->phi(), 
                         p->xProd(), p->yProd(), p->zProd(), p->tProd(), 
                         p->e(), p->eT() ); 

      event.push_back( seed );

      MSG_INFO( "Adding particle: id = " << p->id() << " eta = " << p->eta() << " phi = " << p->phi() << " Et = " << p->eT() );
    
    }// Add particle

  }// Loop over all generated particles

  return StatusCode::SUCCESS;
}



StatusCode ParticleGun::finalize()
{
  MSG_INFO( "Finalize the ParticleGun Event." );
  m_generator.stat();
  return StatusCode::SUCCESS;
}




void ParticleGun::fillParticle( Pythia8::Event& event,
                                int id, double emin, double emax, double thetaIn, double phiIn,
                                ParticleData& pdt, bool atRest = false, bool hasLifetime = false) 
{
  auto rndm   = m_generator.rndm;
  
  // Reset event record to allow for new event.
  event.reset();

  for ( int n=0; n<m_numberOfParticles; ++n){
  

    double ee = ( emax - emin ) * rndm.flat() + emin;
    MSG_INFO("Fill particle... E = " << ee);

    // Select particle mass; where relevant according to Breit-Wigner.
    double mm = pdt.mSel(id);
    double pp = sqrtpos(ee*ee - mm*mm);

    // Special case when particle is supposed to be at rest.
    if (atRest) {
      ee = mm;
      pp = 0.;
    }

    // Angles as input or uniform in solid angle.
    double cThe, sThe, phi;
    if (thetaIn >= 0.) {
      cThe = cos(thetaIn);
      sThe = sin(thetaIn);
      phi  = phiIn;
    } else {
      cThe = 2. * rndm.flat() - 1.;
      sThe = sqrtpos(1. - cThe * cThe);
      phi = 2. * M_PI * rndm.flat();
    }

    // Store the particle in the event record.
    int iNew = event.append( id, 1, 0, 0, pp * sThe * cos(phi),
      pp * sThe * sin(phi), pp * cThe, ee, mm);

    // Generate lifetime, to give decay away from primary vertex.
    if (hasLifetime) event[iNew].tau( event[iNew].tau0() * rndm.exp() );

  }// Fill n particles
  MSG_INFO("Fill particle... done");

}




