
#include "EventInfo/EventInfo.h"
#include "G4Kernel/ParticleGun.h"
#include "G4Kernel/EventLoop.h"
#include "G4RunManager.hh"
#include "G4LorentzVector.hh"
#include "G4Event.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4TransportationManager.hh"

#include "G4Kernel/constants.h"

ParticleGun::ParticleGun( std::string name):
    IMsgService(name),
    PrimaryGenerator(),
    m_gun(nullptr),
    m_evt(0),
    m_generator(0)
{
  declareProperty( "EventKey"           , m_eventKey="EventInfo"    );
  declareProperty( "Particle"           , m_particle="e-"           );
  declareProperty( "Energy"             , m_energy=100*1e3          );
  declareProperty( "EnergyDist"         , m_energyDist="Gauss"      );
  declareProperty( "Sigma"              , m_sigma=1.0               );
  declareProperty( "EtaMax"             , m_etamax=1.0              );
}



ParticleGun::~ParticleGun()
{
  if(m_gun) delete m_gun;
}


StatusCode ParticleGun::initialize()
{

  MSG_INFO( "Initialize the particle gun" );
  
  //m_gun = new G4GeneralParticleSource();
  m_gun = new G4ParticleGun();

  return StatusCode::SUCCESS;
}


StatusCode ParticleGun::finalize()
{
  return StatusCode::SUCCESS;
}





// Call by geant
void ParticleGun::GeneratePrimaryVertex( G4Event* anEvent )
{
  m_evt++;
  EventLoop *loop = static_cast<EventLoop*> (G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  SG::WriteHandle<xAOD::EventInfoContainer>  event(m_eventKey, loop->getContext());
  event.record( std::unique_ptr<xAOD::EventInfoContainer>( new xAOD::EventInfoContainer() ) );
  xAOD::EventInfo *evt = new xAOD::EventInfo();
 


  auto pos = RandomPos( m_etamax );
  auto *particle = G4ParticleTable::GetParticleTable()->FindParticle(m_particle);
  float pp = m_generator.Gaus( m_energy, m_sigma );
  float mass = particle->GetPDGMass();
  float ekin = std::sqrt(pp*pp+mass*mass) - mass;
  int pdgid= particle->GetParticleDefinitionID();
  float et = pos.PseudoRapidity() != 0.0 ? ekin / std::cosh(std::fabs(pos.PseudoRapidity())) : 0.0;

  MSG_INFO( "Ekin = " << ekin << " Et = " << et);

  m_gun->SetParticleDefinition(particle);
  m_gun->SetParticleEnergy(ekin);
  m_gun->SetParticleTime( 0 );
  m_gun->SetParticleMomentumDirection(G4ThreeVector(pos.x(),pos.y(),pos.y()));
  m_gun->GeneratePrimaryVertex(anEvent);
  m_gun->SetParticleTime( (special_bcid_for_truth_reconstruction * 25.) * c_light/mm );// in ns
  m_gun->GeneratePrimaryVertex(anEvent);

  
  xAOD::seed_t seed{et, (float)pos.PseudoRapidity(), (float)pos.Phi(), pdgid};
  evt->setEventNumber( m_evt );
  evt->setAvgmu( 0.0 );
  evt->push_back(seed);
  event->push_back(evt);



  MSG_INFO( "eta = " << pos.PseudoRapidity() << " phi = " << pos.Phi() << " pdgid = " << pdgid);
  MSG_INFO( "Event id         : " << evt->eventNumber() );
}


bool ParticleGun::CheckVertexInsideWorld(const G4ThreeVector& pos) const
{
  G4Navigator* navigator= G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  G4VPhysicalVolume* world= navigator->GetWorldVolume();
  G4VSolid* solid= world-> GetLogicalVolume()->GetSolid();
  EInside qinside= solid->Inside(pos);
  if( qinside != kInside) return false;
  else return true;
}


TVector3 ParticleGun::RandomPos( float etamax )
{
  float eta=5.0;
  float x,y,z;
  while( eta > etamax ){
    x = m_generator.Gaus(0.0, 3.5); y = m_generator.Gaus(0.0, 3.5); z = m_generator.Gaus(0.0, 3.5);  
    TVector3 pos(x, y, z);
    eta = std::abs(pos.PseudoRapidity());
    MSG_INFO( "-----------------___________> "<<eta);
  }
  return TVector3(x,y,z);
}

PrimaryGenerator* ParticleGun::copy()
{
  auto *gun = new ParticleGun( getLogName() );
  gun->setProperty( "EventKey", m_eventKey );
  gun->setProperty( "Particle", m_particle );
  gun->setProperty( "Energy", m_energy );
  gun->setProperty( "EnergyDist", m_energyDist );
  gun->setProperty( "Sigma", m_sigma );
  return gun;
}



