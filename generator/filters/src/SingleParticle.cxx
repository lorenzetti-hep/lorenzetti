
#include "SingleParticle.h"
#include "helper.h"
#include <cmath>
#include "Pythia8/Pythia.h"
#include "../../evtgen/src/Pythia8Gen.h"

using namespace Pythia8;
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
  declareProperty( "DoRangedPhi"      , m_doRangedPhi=false         );
  declareProperty( "PhiMin"           , m_phiMin=-3.14              );
  declareProperty( "PhiMax"           , m_phiMax=3.14               );
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

  // TODO: This is a hack. Force to get pythia8 object 
  Pythia8Gen *p8gen = static_cast<Pythia8Gen*>(generator());
  auto gun = p8gen->core();



  
  // Fix energy or flat distribution between min->max 
  float energy = m_energy>0.0 ? m_energy : ( m_energyMin + (m_energyMax-m_energyMin)*generator()->random_flat());
  float eta = m_doRangedEta? ( m_etaMin + (m_etaMax-m_etaMin)*generator()->random_flat()) : m_eta;
  float phi = m_doRangedPhi? ( m_phiMin + (m_phiMax-m_phiMin)*generator()->random_flat()) : m_phi;

  gun->event.reset();

  MSG_INFO("Fill particle: eta = " << eta << " phi = " << phi << " energy = " << energy);
  fill( gun, m_pdgid, energy/1e3, eta, phi, m_atRest, m_hasLifetime);
  
  

  // Generate main event. Quit if too many failures.
  if (!gun->next()) {
    if (m_iAbort++>m_nAbort){
      MSG_ERROR("Event generation aborted prematurely, owing to error in main event!" );
      throw AbortPrematurely();
    }
  }

  double weight = gun->info.mergingWeight();
  double evtweight = gun->info.weight();
  weight *= evtweight;

  // Do not print zero-weight events.
  if ( weight == 0. ) {
    MSG_WARNING("Pythia generation return weight zero.");
    return StatusCode::FAILURE;
  }


  for ( int i=0; i<gun->event.size(); ++i ){

    auto *p = &gun->event[i];
    ParticleData* pdt = &gun->particleData;
    

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


      ctx.push_back( seed );
      MSG_INFO( "Adding particle: id = " << p->id() << " eta = " << p->eta() << " phi = " << p->phi() << " Et = " << p->eT() );
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





void SingleParticle::fill(Pythia8::Pythia *gun, int id, float energy, float etaIn, float phiIn, bool atRest=false, bool hasLifetime=false)
{

  MSG_INFO("Fill particle with ID " << id);
  float thetaIn = acos( tanh(etaIn ) );
  ParticleData& pdt = gun->particleData;
  
  // Select particle mass; where relevant according to Breit-Wigner.
  float ee = energy;
  float mm = pdt.mSel(id);
  float pp = sqrtpos(ee*ee - mm*mm);

  // Special case when particle is supposed to be at rest.
  if (atRest) {
    ee = mm;
    pp = 0.;
  }

  // Angles as input or uniform in solid angle.
  float cThe, sThe, phi;
  cThe = cos(thetaIn);
  sThe = sin(thetaIn);
  phi  = phiIn;

  // Store the particle in the event record.

  MSG_INFO( "Storing particle into the event record..." );

  int iNew = gun->event.append( id, 1, 0, 0, pp * sThe * cos(phi),
    pp * sThe * sin(phi), pp * cThe, ee, mm);
  
  MSG_INFO("Particle stored with i = " << iNew);
  // Generate lifetime, to give decay away from primary vertex.
  if (hasLifetime) gun->event[iNew].tau( gun->event[iNew].tau0() * gun->rndm.exp() );
}


