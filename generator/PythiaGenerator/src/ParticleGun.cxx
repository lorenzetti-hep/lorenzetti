
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
  declareProperty( "EnergyMin"        , m_energyMin=0               );
  declareProperty( "EnergyMax"        , m_energyMax=100*1e3         );
  declareProperty( "Particle"         , m_pdgid=11                  );
  declareProperty( "HasLifetime"      , m_hasLifetime=false         );
  declareProperty( "AtRest"           , m_atRest=false              );
  declareProperty( "DoRangedEta"      , m_doRangedEta=false         );
  declareProperty( "EtaMin"           , m_etaMin=-2.5                  );
  declareProperty( "EtaMax"           , m_etaMax=2.5                   );
  

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
  m_nAbort = 10;
  return StatusCode::SUCCESS;
}



StatusCode ParticleGun::execute( generator::Event &event )
{

  MSG_INFO( "Start particle gun...");


  // Fix energy or flat distribution between min->max 
  float energy = m_energy>0.0 ? m_energy : ( m_energyMin + (m_energyMax-m_energyMin)*m_generator.rndm.flat());
  float eta = m_doRangedEta? ( m_etaMin + (m_etaMax-m_etaMin)*m_generator.rndm.flat()) : m_eta;


  clearParticles();
  fillParticle( m_pdgid, energy/1e3, eta, m_phi, m_atRest, m_hasLifetime);
  
  
  MSG_INFO("Generate particles...");
  
  // Generate main event. Quit if too many failures.
  if (!m_generator.next()) {
    if (m_iAbort++>m_nAbort){
      MSG_ERROR("Event generation aborted prematurely, owing to error in main event!" );
      throw AbortPrematurely();
    }
  }

  double weight = m_generator.info.mergingWeight();
  double evtweight = m_generator.info.weight();
  weight *= evtweight;

  // Do not print zero-weight events.
  if ( weight == 0. ) {
    MSG_WARNING("Pythia generation return weight zero.");
    return StatusCode::FAILURE;
  }


  for ( int i=0; i<m_generator.event.size(); ++i ){

    auto *p = &m_generator.event[i];
    ParticleData* pdt = &m_generator.particleData;
    

    // Find any unrecognized particle codes.
    int id = p->id();

    if (id == 0 || !pdt->isParticle(id)){
      MSG_WARNING("Unknown code id = " << id );
      continue;
    }


    // Study final-state particles.
    if (p->isFinal()) {
    
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


void ParticleGun::clearParticles()
{
  // Reset event record to allow for new event.
  m_generator.event.reset();
}


void ParticleGun::fillParticle( int id, double energy, double etaIn, double phiIn, bool atRest = false, bool hasLifetime = false) 
{


  double thetaIn = acos( tanh(etaIn ) );
  ParticleData& pdt = m_generator.particleData;
  
  // Select particle mass; where relevant according to Breit-Wigner.
  double ee = energy;
  double mm = pdt.mSel(id);
  double pp = sqrtpos(ee*ee - mm*mm);
  MSG_INFO("Fill particle... E = " << ee);


  // Special case when particle is supposed to be at rest.
  if (atRest) {
    ee = mm;
    pp = 0.;
  }

  // Angles as input or uniform in solid angle.
  double cThe, sThe, phi;
  cThe = cos(thetaIn);
  sThe = sin(thetaIn);
  phi  = phiIn;

  // Store the particle in the event record.
  int iNew = m_generator.event.append( id, 1, 0, 0, pp * sThe * cos(phi),
    pp * sThe * sin(phi), pp * cThe, ee, mm);

  // Generate lifetime, to give decay away from primary vertex.
  if (hasLifetime) m_generator.event[iNew].tau( m_generator.event[iNew].tau0() * m_generator.rndm.exp() );
}




