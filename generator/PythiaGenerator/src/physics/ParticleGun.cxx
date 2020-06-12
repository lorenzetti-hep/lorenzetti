
#include "ParticleGun.h"
#include <algorithm>
#include "Pythia8/Pythia.h"
#include "Pythia8/HeavyIons.h"
#include "Pythia8Plugins/HepMC2.h"
#include "fastjet/PseudoJet.hh"
#include "fastjet/JetDefinition.hh"
#include "fastjet/ClusterSequence.hh"
#include "../tools/ParticleFilter.h"
#include "EventInfo/EventInfo.h"


using namespace Pythia8;

ParticleGun::ParticleGun(): 
  IMsgService("ParticleGun"), 
  Physics(),
  m_abort(0),
  m_nAbort(3)

{
  declareProperty( "Eta"              , m_eta=0.0                   );
  declareProperty( "Phi"              , m_phi=0.0                   );
  declareProperty( "EnergyMin"        , m_energyMin=0.0             );
  declareProperty( "EnergyMax"        , m_energyMax=0.0             );
  declareProperty( "Particle"         , m_pdgid=11                  );
  declareProperty( "NumberOfParticles", m_numberOfParticles=0       );
  declareProperty( "HasLifetime"      , m_hasLifetime=false         );
  declareProperty( "AtRest"           , m_atRest=false              );
  declareProperty( "Seed"             , m_seed=0 /* clock system */ );
  declareProperty( "OutputLevel"      , m_outputLevel=1             );


}



StatusCode ParticleGun::initialize()
{
  MSG_INFO( "Initializing the ParticleGun..." );
  std::stringstream cmdseed; cmdseed << "Random:seed = " << m_seed;
  
  // Read in commands from external file.
  m_pythia.readString("Random:setSeed = on");
  m_pythia.readString(cmdseed.str());
  // Key requirement: switch off ProcessLevel, and thereby also PartonLevel.
  m_pythia.readString("ProcessLevel:all = off");
  m_pythia.readString("Random:setSeed = on");
  m_pythia.readString("Random:seed = 0");
  // Switch off automatic event listing in favour of manual.
  m_pythia.readString("Next:numberShowInfo = 1");
  m_pythia.readString("Next:numberShowProcess = 1");
  m_pythia.readString("Next:numberShowEvent = 1");

  //m_nAbort = m_pythia.mode("Main:timesAllowErrors");
  // Initialization for main (LHC) event
  m_pythia.init();
  
  m_theta = acos( tanh( m_eta ) );

  return StatusCode::SUCCESS;
}



StatusCode ParticleGun::run( std::vector<xAOD::seed_t> &seed_vec, std::vector<std::vector<Particle*>>& particles )
{

  auto event = m_pythia.event;

  ParticleData& pdt = m_pythia.particleData;


  for (int n=0; n<m_numberOfParticles; n++)
    fillParticle( event, m_pdgid, m_energyMin/1e3, m_energyMax/1e3, m_theta, m_phi, pdt, m_atRest, m_hasLifetime);
  
  
  // Generate main event. Quit if too many failures.
  if (!m_pythia.next()) {
    if (m_abort++>m_nAbort){
      MSG_ERROR("Event generation aborted prematurely, owing to error in main event!" );
      throw AbortPrematurely();
    }
  }

  double weight = m_pythia.info.mergingWeight();
  double evtweight = m_pythia.info.weight();
  weight *= evtweight;

  // Do not print zero-weight events.
  if ( weight == 0. ) {
    MSG_WARNING("Pythia generation return weight zero.");
    return StatusCode::FAILURE;
  }


  for ( int i=0; i<event.size(); ++i ){

    auto *p = &event[i];
    // Find any unrecognized particle codes.
    int id = p->id();

    if (id == 0 || !pdt.isParticle(id)){
      MSG_WARNING(" Error! Unknown code id = " << id );
      continue;
    }


    // Study final-state particles.
    if (p->isFinal()) {

      std::vector<Particle*> vec;
      vec.push_back(p);
      particles.push_back( vec );
    
      seed_vec.push_back( xAOD::seed_t{ (float)p->eT(), 
                                        (float)p->eta(), 
                                        (float)p->phi(), 
                                        (float)p->px(), 
                                        (float)p->py(), 
                                        (float)p->pz(), 
                                        p->id() } );

      MSG_INFO( "Adding particle: id = " << p->id() << " eta = " << p->eta() << " phi = " << p->phi() << " Et = " << p->eT() );
    
    }// Add particle

  }// Loop over all generated particles

  return StatusCode::SUCCESS;
}



StatusCode ParticleGun::finalize()
{
  MSG_INFO( "Finalize the ParticleGun Event." );
  m_pythia.stat();
  return StatusCode::SUCCESS;
}




void ParticleGun::fillParticle( Event& event,
                                int id, double emin, double emax, double thetaIn, double phiIn,
                                ParticleData& pdt, bool atRest = false, bool hasLifetime = false) 
{
  auto rndm   = m_pythia.rndm;
  
  // Reset event record to allow for new event.
  event.reset();

  for ( int n=0; n<m_numberOfParticles; ++n){
    
    double ee = ( emax - emin ) * rndm.flat() + emin;

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

}




