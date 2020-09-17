
#include "LLPGun.h"
#include "Pythia8/Pythia.h"


using namespace Pythia8;
using namespace generator;

namespace{

template <typename T> int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

//==========================================================================
double xy_z_to_theta( double r_xy, double z ){
  if (z!=0){
    double theta = atan2(  r_xy , z );
    return theta;
  } else {
    double theta = M_PI/2.;
    return theta;
  }
}

double xy_z_to_tau( double r_xy, double z ){
  return std::sqrt(r_xy*r_xy+z*z);
}

double tau_to_tauPrime( double tau, Particle& mother ){
  return ( mother.m() / mother.pAbs() ) * tau;
}

double eta_to_theta( double eta ){
  return acos(tanh(eta));
}

//==========================================================================
double theta_to_eta( double theta ){
  return std::atanh(std::cos(theta));
}

//==========================================================================
double xy_z_to_eta( double r_xy, double z ){
  return theta_to_eta( xy_z_to_theta( r_xy, z ) );
}

//==========================================================================
// Printer for particle
std::ostream& operator<< ( std::ostream& out, const Particle& pt ){
  int prec = 3;
  out << "    "
       << "     id  name            status     mothers   daughters     colou"
       << "rs      p_x        p_y        p_z        c_x        c_y        c_z         e          m         tau0        tau\n";
       //<< "rs      p_x        p_y        p_z        c_x        c_y        c_z         e          m\n";
  out << setw(11) << pt.id() << "  " << left
       << setw(18) << pt.nameWithStatus(18) << right << setw(4)
       << pt.status() << setw(6) << pt.mother1() << setw(6)
       << pt.mother2() << setw(6) << pt.daughter1() << setw(6)
       << pt.daughter2() << setw(6) << pt.col() << setw(6) << pt.acol()
       << setw(8+prec) << pt.px() << setw(8+prec) << pt.py()
       << setw(8+prec) << pt.pz()
       << setw(8+prec) << pt.xProd() << setw(8+prec) << pt.yProd()
       << setw(8+prec) << pt.zProd()
       << setw(8+prec) << pt.e()
       << setw(8+prec) << pt.m()
       << setw(8+prec) << pt.tau0()
       << setw(8+prec) << pt.tau();
  return out;
}

}

LZT_ATLAS_Info::LZT_ATLAS_Info()
{
  eta_zmax1 = xy_z_to_eta( rmin, zmax1 );
  eta_rmax = xy_z_to_eta( rmax, zmax2 );
}

bool LZT_ATLAS_Info::withinDetector( double r, double z, double eta ){
  if ( abs(eta) < eta_zmax1 ){ // Cut on straight line path to hit the detector
    if ( abs(z) < zmax1 ){ // Cut on detector volume
      if ( abs(r) < emec_max_radii ){
        return true;
      }
    } 
    if ( abs(z) < zmax2 ) {
      if ( ( abs(r) >= emec_max_radii ) && ( abs(r) < detector_max_radii ) ){
        return true;
      }
    }
  }
  return false;
}

double LZT_ATLAS_Info::tauMax()
{
  double theta = eta_to_theta(this->eta_rmax);
  return rmax / ( sin( theta ) );
}

LLPGun::LLPGun(): 
  IMsgService("LLPGun"), 
  Algorithm()
{
  declareProperty( "File"             , m_file=""                   );
  declareProperty( "Energy"           , m_energy=-1                 );
  declareProperty( "EnergyMin"        , m_energyMin=0               );
  declareProperty( "EnergyMax"        , m_energyMax=1e6             );
  declareProperty( "Particle"         , m_pdgid=999999              );
}

LLPGun::~LLPGun()
{;}

StatusCode LLPGun::initialize()
{
  MSG_INFO( "Initializing the LLPGun..." );
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

  // Generator; shorthand for event and particleData.
  m_generator.readFile( m_file );
  // Initialize.
  m_generator.init();

  // Book histograms.
  m_rootFile = new TFile("llp.root", "RECREATE");

  m_decayPositionH       = new TH2F("decay_position","Decay Position;z;r;counts",  100, -m_lzt_atlas_info.zmax2, +m_lzt_atlas_info.zmax2, 100, -m_lzt_atlas_info.rmax, +m_lzt_atlas_info.rmax);
  m_decayPositionAbsAbsH = new TH2F("decay_position_absabs","Decay Position;abs(z);abs(r);counts",  250, 0, +m_lzt_atlas_info.zmax2, 250, 0, +m_lzt_atlas_info.rmax);
  m_decayWrtEtaH         = new TH2F("decay_wrt_eta","Decay w.r.t. eta; #eta; #tau", 50, -m_lzt_atlas_info.eta_zmax1, +m_lzt_atlas_info.eta_zmax1, 100, 0., +m_lzt_atlas_info.tauMax());
  m_energyH              = new TH1F("energy", "leading photon abs(p)", 100, 0, 1000);
  m_sub_energyH          = new TH1F("sub_energy", "subleading photon abs(p)", 100, 0, 1000);
  m_cos_theta_starH      = new TH1F("cos_theta_star", "cos(theta*) leading",100, -1, 1);
  m_sub_cos_theta_starH  = new TH1F("sub_cos_theta_star", "cos(theta*) subleading",100, -1, 1);
  m_tauH                 = new TH1F("tau", "GenericLLP tau",100, 0, m_lzt_atlas_info.tauMax());

  return StatusCode::SUCCESS;
}

void LLPGun::flatTauDist(Pythia8::Event &event, Pythia8::Particle &mother, double tau){
  mother.tau( tau );
  if (!mother.isFinal() && (mother.hasVertex() || mother.tau() > 0.)) {
    Vec4 vDecR = mother.vDec();
    for (int iR = mother.daughter1(); iR <= mother.daughter2(); ++iR){
      auto &child = event[iR];
      child.vProd( vDecR );
    }
  }
}

StatusCode LLPGun::execute( generator::Event &event )
{

  clearParticles();

  //MSG_INFO( "Start particle gun...");
  double z, r, theta, eta;
  while(true){
    // Generate uniform distribution in z, r
    z = -m_lzt_atlas_info.zmax2 + ( 2.*m_lzt_atlas_info.zmax2 * m_generator.rndm.flat() );
    r = -m_lzt_atlas_info.detector_max_radii + ( 2.*m_lzt_atlas_info.detector_max_radii * m_generator.rndm.flat() );
    // Compute eta
    theta = xy_z_to_theta( r, z );
    eta = theta_to_eta( theta );
    // Use rejection MC method
    if ( m_lzt_atlas_info.withinDetector( r, z, eta ) ){
      break;
    }
  }

  // Fix energy or flat distribution between min->max 
  const double energy = m_energy>0.0 ? m_energy : ( m_energyMin + (m_energyMax-m_energyMin)*m_generator.rndm.flat());

  const double phi = - M_PI + 2*M_PI*m_generator.rndm.flat();

  fillParticle( m_pdgid, energy/1e3, theta, phi );
  
  //MSG_INFO("Generate particles...");
  
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

  // Loop over all particles.
  std::vector<std::size_t> phIdxs(0);
  std::size_t genericLLP_idx(0);
  for (int i = 0; i < m_generator.event.size(); ++i) 
  {
    auto &part = m_generator.event[i];
    int id = part.id();
    if (part.isFinal() && id == 22 ) {
      phIdxs.push_back(i);
    } else if ( id == m_pdgid ) {
      genericLLP_idx = i;
    }
  }

  // Fix tau distribution
  const double tau = xy_z_to_tau( r, z );
  const double tauPrime = tau_to_tauPrime( tau, m_generator.event[genericLLP_idx] );
  flatTauDist( m_generator.event, m_generator.event[genericLLP_idx], tauPrime );

  auto &llp = m_generator.event[genericLLP_idx];

  if ( !phIdxs.empty() ){
    auto seed = generator::Seed( llp.eta(), llp.phi() );
    auto comp = m_generator.event[phIdxs[0]].pAbs() > m_generator.event[phIdxs[1]].pAbs();
    size_t leading = ( comp )?phIdxs[0]:phIdxs[1];
    size_t subleading = ( comp )?phIdxs[1]:phIdxs[0];
    auto leading_photon = m_generator.event[leading];
    auto subleading_photon = m_generator.event[subleading];
    seed.emplace_back( 1, 0, leading_photon.id(), leading_photon.px(), leading_photon.py(), leading_photon.pz(), leading_photon.eta(), leading_photon.phi(), 
                       leading_photon.xProd(), leading_photon.yProd(), leading_photon.zProd(), leading_photon.tProd(), 
                       leading_photon.e(), leading_photon.eT() ); 
    seed.emplace_back( 1, 0, subleading_photon.id(), subleading_photon.px(), subleading_photon.py(), subleading_photon.pz(), subleading_photon.eta(), subleading_photon.phi(), 
                       subleading_photon.xProd(), subleading_photon.yProd(), subleading_photon.zProd(), subleading_photon.tProd(), 
                       subleading_photon.e(), subleading_photon.eT() ); 
    event.push_back( seed );
    auto leading_pabs = m_generator.event[leading].pAbs();
    auto sub_leading_pabs = m_generator.event[subleading].pAbs();
    auto leading_p = m_generator.event[leading].p();
    leading_p.bstback(llp.p(), llp.m());
    auto leading_costheta = costheta( llp.p(), leading_p );
    auto sub_leading_p = m_generator.event[subleading].p();
    sub_leading_p.bstback(llp.p(), llp.m());
    auto sub_leading_costheta = costheta( llp.p(), sub_leading_p );
    m_energyH->Fill( leading_pabs );
    m_sub_energyH->Fill( sub_leading_pabs );
    m_cos_theta_starH->Fill( leading_costheta );
    m_sub_cos_theta_starH->Fill( sub_leading_costheta );
    m_tauH->Fill( tau );
    m_decayPositionH->Fill( z, r );
    m_decayPositionAbsAbsH->Fill( abs(z), abs(r) );
    m_decayWrtEtaH->Fill( eta, tau );
    MSG_DEBUG( "Adding particle: id = " << llp.id() << " eta = " << llp.eta() << " phi = " << llp.phi() << " E = " << llp.e() << " Tau = " << llp.tau() );
    MSG_DEBUG( "Leading photon " << " eta = " << leading_p.eta() << " phi = " << leading_p.phi() << " E = " << leading_p.e() );
    MSG_DEBUG( "Subleading photon " << " eta = " << sub_leading_p.eta() << " phi = " << sub_leading_p.phi() << " E = " << sub_leading_p.e() );
  }
  // End of event loop.

  return StatusCode::SUCCESS;
}

StatusCode LLPGun::finalize()
{
  MSG_INFO( "Finalize the LLPGun Event." );
  m_generator.stat();
  m_rootFile->Write();
  m_rootFile->Close();
  m_rootFile->Delete();
  return StatusCode::SUCCESS;
}


void LLPGun::clearParticles()
{
  // Reset event record to allow for new event.
  m_generator.event.reset();
}


void LLPGun::fillParticle( int id, double ee, double thetaIn, double phiIn)
{

  ParticleData& pdt = m_generator.particleData;
  
  // Select particle mass; where relevant according to Breit-Wigner.
  double mm = pdt.mSel(id);
  double pp = sqrtpos(ee*ee - mm*mm);
  //MSG_INFO("Fill particle... E = " << ee);

  // Angles as input or uniform in solid angle.
  double cThe, sThe, phi;
  cThe = cos(thetaIn);
  sThe = sin(thetaIn);
  phi  = phiIn;

  // Store the particle in the event record.
  m_generator.event.append( id, 1, 0, 0, pp * sThe * cos(phi), pp * sThe * sin(phi), pp * cThe, ee, mm);

}
