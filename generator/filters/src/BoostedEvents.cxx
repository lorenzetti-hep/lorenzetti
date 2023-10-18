
#include "BoostedEvents.h"
#include "helper.h"
#include "TMath.h"
#include "Pythia8/Pythia.h"
#include "../../evtgen/src/Pythia8Gen.h"

using namespace Pythia8;
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


  generator()->clear();

  // TODO: This is a hack. Force to get pythia8 object 
  Pythia8Gen *p8gen = static_cast<Pythia8Gen*>(generator());
  auto gun = p8gen->core();



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
    fill( gun, m_pdgid, energy, eta, phi, m_atRest, m_hasLifetime);
  }
  
  



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

    const auto main_event_t = sample_t();
    const auto main_event_z = sample_z();

    // Study final-state particles.
    if (p->isFinal()) {
    

      // Search for the associated seed into the event record
      for (auto& seed : *ctx)
      {
        // Get the associated seed for this generated particle
        // Add the particle into the seed particle list
        if (dR( p->eta(), p->phi(), seed.eta(), seed.phi() ) <= m_deltaR )
        {
          seed.emplace_back( 1, 0, p->id(), p->px(), p->py(), p->pz(), p->eta(), p->phi(), 
                         p->xProd(), p->yProd(), p->zProd() + main_event_z, p->tProd() + main_event_t, 
                         p->e(), p->eT() ); 

          MSG_INFO( "Adding particle: id = " << p->id() << 
                    " eta = " << p->eta() << 
                    " phi = " << p->phi() << 
                    " Et = "  << p->eT() );
        }// delta R inside
      }// Loop over seeds
    }// Is final

  }// Loop over particles





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



void BoostedEvents::fill(Pythia8::Pythia *gun, int id, float energy, float etaIn, float phiIn, bool atRest=false, bool hasLifetime=false)
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



