// main18.cc is a part of the PYTHIA event generator.
// Copyright (C) 2019 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL v2 or later, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

//==========================================================================
// This program creates a jet for each main event and a set of minbias events
// for a bunch-cross (BC) window.
//==========================================================================
// NOTE: Pythia always use GeV and mm (time also in mm)

#include <algorithm>
#include <string>
#include <random>

#include "Pythia8/Pythia.h"
#include "Pythia8/HeavyIons.h"

#include "Pythia8Plugins/HepMC2.h"

// ROOT includes
#include "TTree.h"
#include "TFile.h"
#include "TVector3.h"
#include "TH1F.h"
#include "TH2F.h"

#include "fastjet/PseudoJet.hh"
#include "fastjet/JetDefinition.hh"
#include "fastjet/ClusterSequence.hh"

static const double c_light = 2.99792458e+8; // m/s

using namespace Pythia8;

// Initialize random generator
namespace{
std::random_device rd;
std::mt19937 eng(rd());

// Method to pick a number according to a Poissonian distribution.
int poisson(double nAvg, Rndm& rndm) {
  // Set maximum to avoid overflow.
  const int NMAX = 300;

  // Random number.
  double rPoisson = rndm.flat() * exp(nAvg);

  // Initialize.
  double rSum  = 0.;
  double rTerm = 1.;

  // Add to sum and check whether done.
  for (int i = 0; i < NMAX; ) {
    rSum += rTerm;
    if (rSum > rPoisson) return i;

    // Evaluate next term.
    ++i;
    rTerm *= nAvg / i;
  }

  // Emergency return.
  return NMAX;
}


std::vector<float>* nvf(){
  return new std::vector<float>();
}
std::vector<int>* nvi(){
  return new std::vector<int>();
}
//==========================================================================
// Printer for particle
std::ostream& operator<< ( std::ostream& out, const Particle& pt ){
  int prec = 3;
	out << "    "
       << "     id  name            status     mothers   daughters     colou"
       << "rs      p_x        p_y        p_z        c_x        c_y        c_z         e          m \n";
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
       << setw(8+prec) << pt.m();
  return out;
}

static std::size_t level(1);

std::ostream& operator<< ( std::ostream& out, const std::vector<fastjet::PseudoJet>& pt );
std::ostream& operator<< ( std::ostream& out, const fastjet::PseudoJet& pt );

std::ostream& operator<< ( std::ostream& out, const std::vector<fastjet::PseudoJet>& pt ){
  for( auto s : pt ){
    out << s;
  }
  return out;
}

//==========================================================================
// Printer for PseudoJet
std::ostream& operator<< ( std::ostream& out, const fastjet::PseudoJet& s ){
  for( std::size_t i = 0; i < level; ++i ){
    out << "-";
  }
  out << "|x: " << s.px() << " |y: " << s.py() << " |z: " << s.pz() << " |eta: " << s.eta() << " |phi: " << s.phi() << " |E: " << s.E() << " |pt: " << s.pt() << " |Et: " << s.Et() << " |abs(eta): " << std::abs(s.eta()) << endl;
  if ( s.has_constituents()){
    ++::level;
    const auto& c = s.constituents();
    if ( c.size() > 1 ){
      out  << c;
    }
    --::level;
  }
  return out;
}


//==========================================================================
// Printer for particle
inline
std::ostream& operator<< ( std::ostream& out, const Particle* pt ){
  return operator<<( out, *pt );
}

//==========================================================================
// Printer for particle vector
std::ostream& operator<< ( std::ostream& out, const std::vector< Particle* >& vec )
{
  int prec = 3;
	out << "    no    "
      << "     id  name            status     mothers   daughters     colou"
      << "rs      p_x        p_y        p_z         e          m \n";
  // Basic line for a particle, always printed.
  for ( std::size_t i = 0; i < vec.size(); ++i ){
    const auto& pt = *vec[i];
    out << setw(6) << i << setw(11) << pt.id() << "  " << left
        << setw(18) << pt.nameWithStatus(18) << right << setw(4)
        << pt.status() << setw(6) << pt.mother1() << setw(6)
        << pt.mother2() << setw(6) << pt.daughter1() << setw(6)
        << pt.daughter2() << setw(6) << pt.col() << setw(6) << pt.acol()
        << setw(8+prec) << pt.px() << setw(8+prec) << pt.py()
        << setw(8+prec) << pt.pz() << setw(8+prec) << pt.e()
        << setw(8+prec) << pt.m() << "\n";
  }
  return out;
}
}

//==========================================================================

// The ParticleFilter class.

// The constructor takes the following arguments
// select = 1 : keep only final particles.
//        = 2 : keep only final visible particles (i.e. not neutrinos).
//        = 3 : keep only final charged particles.
// etaMax (default = 50) : keep only particles with pseudorapidity
//        |eta| < etaMax.
// pTminCharged (default = 0) : keep a charged particle only if
//        its transverse momentum pT < pTminCharged.
// pTminNeutral (default = 0) : keep a neutral particle only if
//        its transverse momentum pT < pTminNeutral.

// Main methods:
// filter( event) takes an event record as input and analyzes it.
// size() returns the number of particles kept.
// index(i) returns the index in the full event of the i'th kept particle.
// particlePtr(i) returns a pointer to the i'th kept particle.
// particleRef(i) returns a reference to the i'th kept particle.
// list() gives a listing of the kept particles only.

class ParticleFilter {

public:

  // Constructor sets properties of filter.
  ParticleFilter( int selectIn, double etaMaxIn = 50.,
    double pTminChargedIn = 0., double pTminNeutralIn = 0.)
    : select(selectIn), etaMax(etaMaxIn), pTminCharged(pTminChargedIn),
    pTminNeutral(pTminNeutralIn) {}

  // Analysis of each new event to find acceptable particles.
  void filter(Event& event);

  // Return size of array, and index of a particle.
  int size()       const {return keptPtrs.size();}
  int index(int i) const {return keptIndx[i];}

  // Return pointer or reference to a particle.
  Particle* particlePtr(int i) {return  keptPtrs[i];}
  Particle& particleRef(int i) {return *keptPtrs[i];}

  // List kept particles only.
  void list(ostream& os = cout);

  std::vector<Particle*>& getParticlesRef() { return keptPtrs; }

private:

  // Filter properties, set by constructor.
  int    select;
  double etaMax, pTminCharged, pTminNeutral;

  // Kept particle indices and pointers, referring to original event.
  vector<int>       keptIndx;
  vector<Particle*> keptPtrs;

};

//--------------------------------------------------------------------------

// The det_acc_filter method.

void ParticleFilter::filter(Event& event) {

  // Reset arrays in preparation for new event.
  keptIndx.resize(0);
  keptPtrs.resize(0);

  // Loop over all particles in the event record.
  for (int i = 0; i < event.size(); ++i) {

    // Skip if particle kind selection criteria not fulfilled.
    if (!event[i].isFinal()) continue;
    if (select == 2 && !event[i].isVisible()) continue;
    bool isCharged = event[i].isCharged();
    if (select == 3 && !isCharged) continue;

    // Skip if too large pseudorapidity.
    if (abs(event[i].eta()) > etaMax) continue;

    // Skip if too small pT.
    if       (isCharged && event[i].pT() < pTminCharged) continue;
    else if (!isCharged && event[i].pT() < pTminNeutral) continue;

    // Add particle to vectors of indices and pointers.
    keptIndx.push_back( i );
    keptPtrs.push_back( &event[i] );

  // End of particle loop. Done.
  }

}

//--------------------------------------------------------------------------

// The list method: downscaled version of Event::list.

void ParticleFilter::list(ostream& os) {

  // Header.
  os << "\n --------  PYTHIA Event Listing  (filtered)  ------------------"
     << "-----------------------------------------------------------------"
     << "----\n \n    no        id   name            status     mothers  "
     << " daughters     colours      p_x        p_y        p_z         e  "
     << "        m \n";

  // At high energy switch to scientific format for momenta.
  double eSum = 0.;
  for (int iKept = 0; iKept < size(); ++iKept) eSum += keptPtrs[iKept]->e();
  bool useFixed = (eSum < 1e5);

  // Listing of kept particles in event.
  for (int iKept = 0; iKept < size(); ++iKept) {
    int i = keptIndx[iKept];
    Particle& pt = *keptPtrs[iKept];

    // Basic line for a particle, always printed.
    os << setw(6) << i << setw(10) << pt.id() << "   " << left
       << setw(18) << pt.nameWithStatus(18) << right << setw(4)
       << pt.status() << setw(6) << pt.mother1() << setw(6)
       << pt.mother2() << setw(6) << pt.daughter1() << setw(6)
       << pt.daughter2() << setw(6) << pt.col() << setw(6) << pt.acol()
       << ( (useFixed) ? fixed : scientific ) << setprecision(3)
       << setw(11) << pt.px() << setw(11) << pt.py() << setw(11)
       << pt.pz() << setw(11) << pt.e() << setw(11) << pt.m() << "\n";
  }

  // Listing finished.
  os << "\n --------  End PYTHIA Event Listing  ----------------------------"
     << "-------------------------------------------------------------------"
     << endl;
}

class NotInterestingEvent {};

//==========================================================================
void getJets( const std::vector<Particle*>& particles, const double pt_filter, const double eta_filter,
    std::vector<fastjet::PseudoJet>& inclusive_jets, std::vector<std::vector<Particle*>>& jet_pythia_particles){

  // Make one-to-one correspondence between fastjet PseudoJet and Particle
  std::vector<fastjet::PseudoJet> input_particles;
  std::map< std::size_t, Particle* > local_barcode_map;
  std::size_t barcode(0);
  for ( const auto part : particles ) {
    fastjet::PseudoJet substruct_cand(part->px(),part->py(),part->pz(),part->e());
    substruct_cand.set_user_index( barcode );
    input_particles.push_back( substruct_cand );
    local_barcode_map.insert( std::pair< std::size_t, Particle* >( barcode++, part ) );
  }

  // Clusterize hadrons
  fastjet::JetDefinition jet_def(fastjet::antikt_algorithm,0.4);
  fastjet::ClusterSequence clust_seq(input_particles,jet_def);
  // Apply filters
  inclusive_jets = sorted_by_pt( clust_seq.inclusive_jets() );
  inclusive_jets.erase(std::remove_if(inclusive_jets.begin(),
                            inclusive_jets.end(),
                            [pt_filter,eta_filter](fastjet::PseudoJet& j){return (std::abs(j.eta()) > eta_filter) || (j.pt() < pt_filter);}
                                     ),
                       inclusive_jets.end());

  if ( inclusive_jets.empty() ){
    throw NotInterestingEvent();
  }

  // Map back from the PseudoJet to particles
  jet_pythia_particles.clear();

  for ( const auto j : inclusive_jets ){
    const auto& substructs = j.constituents();
    auto lp = std::vector<Particle*>(substructs.size());
    std::size_t idx(0);
    for ( auto& substruct : substructs ){
      lp[idx++] = local_barcode_map[ substruct.user_index() ];
    }
    jet_pythia_particles.push_back(lp);
  }
}

//==========================================================================
void getZee( Event& event, const double pt_filter, const double eta_filter,
    std::vector<std::vector<Particle*>>& zee_pythia_particles ){

  zee_pythia_particles.clear();

  std::vector<Particle*> zee; zee.reserve(2);
  for (int i = 0; i < event.size(); ++i) {
    auto *p = &event[i];
    //std::cout << "pdg_id:" << p->id() << "|eta:" << abs(p->eta()) << "|et:" << p->pT() << std::endl;
    if ( p->id() == 23 ){
      // We got our Z!
      for ( const auto cid : p->daughterListRecursive() ){
        auto *pchild = &event[cid];
        if ( pchild->isFinal() ) {
          if ( pchild->idAbs() == 11 ){
            // Get Zee children
            if ( abs(pchild->eta()) < eta_filter && pchild->pT() > pt_filter ){
              zee.push_back( pchild );
            }
          } else {
            std::cout << "WARN: Event is not a Zee decay" << std::endl;
            std::cout << " === final Z decay === pdg_id:" << pchild->id() << "|eta:" << abs(pchild->eta()) << "|et:" << pchild->pT() << std::endl;
            throw NotInterestingEvent();
          }
        }
      }
      break;
    }
  }
  if ( zee.empty() ){
    throw NotInterestingEvent();
  }

  zee_pythia_particles.push_back( zee );

}

//==========================================================================
// Use the ParticleFilter method to plot some event properties.
int main( int argc, char *argv[] ) {

  std::size_t opt;

  if ( argc < 2 ){
    opt = 1;
  } else {
    opt = std::stoul(argv[1]);
  }

  // If true do Zee, if false do JF17
  bool doZee;
  std::string output_name;
  switch ( opt ) {
    case 0:
      std::cout << "Proceeding to JF17 generation" << std::endl;
      output_name = "pythia_jf17.root";
      doZee = false;
      break;
    case 1:
      std::cout << "Proceeding to Zee generation" << std::endl;
      output_name = "pythia_zee.root";
      doZee = true;
      break;
    default:
      std::cout << "Unknown option, choose either 1 for zee or 0 for jet.";
      exit(1);
  }

  //==========================================================================
  // Filter values
  const int    select   = 2; // Visible particles (no neutrinos)
  const double etaMax = 1.4;
  // Minimum value
  const double jet_pt_filter = 17.; // GeV
  const double zee_pt_filter = 15.; // GeV
  // Average number of pileup events per signal event.
  const double nPileupAvg = 10;
  // The number of BC interfering with the main event. Negative values
  // refer to events before and positive to events after the main event,
  // which is at bc_id = 0.
  const int bc_id_start = -8; 
  const int bc_id_end = 7; 
  const int nWin = bc_id_end - bc_id_start + 1;
  // window around main event to keep minbias particles
  const double mb_delta_eta = 0.22; 
  const double mb_delta_phi = 0.22;
  //==========================================================================

  //==========================================================================
  const double sigma_t = 200 * 1e-12 /*pico seconds*/ * c_light /*m/s*/ * 1e+3 /*mm*/;
  const double sigma_z = 56 /*miliseconds*/;
  //==========================================================================

  //==========================================================================
  // Main generator.
  Pythia pythia;
  // Read in commands from external file.
  pythia.readFile((doZee)?("cmnd/zee_config.cmnd"):("cmnd/jet_config.cmnd"));
  // Number of events to generate, to list, to allow aborts.
  int nEvent      = pythia.mode("Main:numberOfEvents");
  int nAbort      = pythia.mode("Main:timesAllowErrors");
  // Initialization for main (LHC) event
  pythia.init();
  // Minbias generator
  Pythia mb_pythia;
  mb_pythia.readFile((doZee)?("cmnd/minbias_zee_config.cmnd"):("cmnd/minbias_jet_config.cmnd"));
  mb_pythia.init();
  //==========================================================================

  //==========================================================================
  // Declare Event Filter according to specification.
  // 700 MeV cut to remove charged particles trapped inside detector
  ParticleFilter det_acc_filter( select, etaMax + .05, 0.7, 0.05 );
  //==========================================================================

  //==========================================================================
  // Initialize output file
  TFile file(output_name.c_str(),"recreate" );
  // --- Main event data
  TTree tree("particles","Pythia particles event tree");
  // Substructure variables/branches
  double avg_mu;
  std::vector<int> *p_pdg_id(nvi()), *p_bc_id(nvi()), *bc_id_nhits(nvi()), *p_isMain(nvi());
  std::vector<float> *p_px(nvf()), *p_py(nvf()), *p_pz(nvf()),
                     *p_eta(nvf()), *p_phi(nvf()),
                     *p_prod_x(nvf()), *p_prod_y(nvf()), *p_prod_z(nvf()), *p_prod_t(nvf()),
                     *p_e(nvf()), *p_et(nvf()), *bc_id_mu(nvf());
  
  tree.Branch("avg_mu", &avg_mu, "avg_mu/D");
  tree.Branch("p_isMain", &p_isMain);
  tree.Branch("p_pdg_id", &p_pdg_id); 
  tree.Branch("p_bc_id", &p_bc_id); 
  tree.Branch("bc_mu", &bc_id_mu); 
  tree.Branch("bc_id_nhits", &bc_id_nhits);
  tree.Branch("p_px", &p_px); 
  tree.Branch("p_py", &p_py); 
  tree.Branch("p_pz", &p_pz);
  tree.Branch("p_prod_x", &p_prod_x); 
  tree.Branch("p_prod_y", &p_prod_y); 
  tree.Branch("p_prod_z", &p_prod_z);  
  tree.Branch("p_prod_t", &p_prod_t);
  tree.Branch("p_eta", &p_eta); 
  tree.Branch("p_phi", &p_phi);
  tree.Branch("p_e", &p_e); 
  tree.Branch("p_et", &p_et);
  
  
  // --- 1D Minbias histograms
  TH1F th1_mu_bc( "th1_bc_mu", ";bc_mu;count", 100, 0.5, 100.5); 
  TH1F th1_avg_mu( "th1_avg_mu", ";avg_mu;count", 100, 0.5, 100.5 );
  TH1F th1_nhits_bc( "th1_nhits_bc", ";n_{hits}(BC);count", 100, 0., 100 ); 
  TH1F th1_nhits_win( "th1_nhits_win", ";n_{hits}(win);count", 100, 0., 1000 );
  // --- 2D Minbias histograms
  TH2F th2_eperc_eta_bc( "th2_eperc_eta_bc", ";abs(#eta);E%(BC)", 20, 0, 2.5, 60, 0, 100); 
  TH2F th2_eperc_eta_win( "th2_eperc_eta_win", ";abs(#eta);E%(WIN)", 20, 0, 2.5, 60, 0, 100);
  TH2F th2_emb_eta_bc( "th2_emb_eta_bc", ";abs(#eta);E_{MB} [GeV](BC)", 20, 0, 2.5, 60, 0, 10e3); 
  TH2F th2_emb_eta_win( "th2_emb_eta_win", ";abs(#eta);E_{MB} [GeV](WIN)", 20, 0, 2.5, 60, 0, 10e3);
  //==========================================================================

  // Begin event loop.
  int iAbort = 0; int mb_iAbort = 0;
  for (int iEvent = 0; iEvent < nEvent; ++iEvent) {

    // Generate main event. Quit if too many failures.
    if (!pythia.next()) {
      if (++iAbort < nAbort) continue;
      cout << " Event generation aborted prematurely, owing to error in main event!\n";
      break;
    }

    double weight = pythia.info.mergingWeight();
    double evtweight = pythia.info.weight();
    weight *= evtweight;

    // Do not print zero-weight events.
    if ( weight == 0. ) {
      --iEvent; continue;
    }

    // Retrieve jet
    try {
      // Get a jet and its respective pythia particles
      std::vector<fastjet::PseudoJet> jet_vec;
      std::vector<std::vector<Particle*>> pythia_particles_vec;

      if ( doZee ){
        getZee( pythia.event, zee_pt_filter, etaMax, pythia_particles_vec );
      } else {
        // Find final charged particles with |eta| < etaMax
        det_acc_filter.filter( pythia.event );
        getJets( det_acc_filter.getParticlesRef(), jet_pt_filter, etaMax, jet_vec, pythia_particles_vec );
      }

      const auto main_event_t = pythia.rndm.gauss() * sigma_t;
      const auto main_event_z = pythia.rndm.gauss() * sigma_z;

      std::vector<Vec4> pj_vec;
      if ( doZee ) {
        for ( const auto pythia_particles : pythia_particles_vec ) {
          for ( const auto p : pythia_particles ) {
            p_isMain->push_back( 1 );
            p_bc_id->push_back( 0 ); 
            p_pdg_id->push_back( 0 );
            p_px->push_back( p->px() ); 
            p_py->push_back( p->py() ); 
            p_pz->push_back( p->pz() );
            p_eta->push_back( p->eta() ); 
            p_phi->push_back( p->phi() );
            p_prod_x->push_back( 0 ); 
            p_prod_y->push_back( 0 ); 
            p_prod_z->push_back( 0 + main_event_z  ); 
            p_prod_t->push_back( 0 + main_event_t );
            p_e->push_back( p->e() ); 
            p_et->push_back( p->eT() );
          }
        }
      } else {
        for ( const auto jet : jet_vec ) {
          auto pj = Vec4(jet.px(), jet.py(), jet.pz());
          pj_vec.push_back( pj );
          p_isMain->push_back( 1 );
          p_bc_id->push_back( 0 ); 
          p_pdg_id->push_back( 0 );
          p_px->push_back( jet.px() ); 
          p_py->push_back( jet.py() ); 
          p_pz->push_back( jet.pz() );
          p_eta->push_back( jet.eta() ); 
          p_phi->push_back( pj.phi() );
          p_prod_x->push_back( 0 ); 
          p_prod_y->push_back( 0 ); 
          p_prod_z->push_back( 0 + main_event_z  ); 
          p_prod_t->push_back( 0 + main_event_t );
          p_e->push_back( jet.e() ); 
          p_et->push_back( jet.Et() );
        }
      }

      // Retrieve minbias
      double nPileUpMean(0);
      //double mb_e_win(0), mb_electron_photon_e_frac_win(0);
      int nhits_win(0), nsub_jet(0);
      for ( int bc_id = bc_id_start; bc_id <= bc_id_end; ++bc_id ){
        // Generate main event. Quit if too many failures.
        // Select the number of pileup events to generate.
        
        int nPileup = poisson(nPileupAvg, mb_pythia.rndm);
        nPileUpMean += nPileup;

        // Loop over jet data
        if ( bc_id == 0 ) {
          // Jet variables/branches
          // Main event is always in BC 0; pdg is = 0 to sign that this is a RoI
          for ( std::size_t main_idx = 0; main_idx < pythia_particles_vec.size(); ++ main_idx ) {
            const auto pythia_particles = pythia_particles_vec[main_idx];

            //double jet_electron_photon_e_frac = 0.;
            //unsigned int jet_n_part = pythia_particles.size();
            // Fill substruct
            for ( const auto& part : pythia_particles ){
              // Shorthands
              const auto pdg_id = part->id();
              // Obtain substruct data
              // Jet substruct is always in BC 0;
              p_isMain->push_back( 1 );
              p_bc_id->push_back( bc_id ); 
              p_pdg_id->push_back( pdg_id );
              p_px->push_back( part->px() ); 
              p_py->push_back( part->py() ); 
              p_pz->push_back( part->pz() );
              p_eta->push_back( part->eta() ); 
              p_phi->push_back( part->phi() );
              p_prod_x->push_back( part->xProd() ); 
              p_prod_y->push_back( part->yProd() ); 
              p_prod_z->push_back( part->zProd() + main_event_z  ); 
              p_prod_t->push_back( part->tProd() + main_event_t );
              p_e->push_back( part->e() ); 
              p_et->push_back( part->eT() );
              //if ( pdg_id == 22 || abs(pdg_id) == 11 ){
              //  jet_electron_photon_e_frac += part->e();
              //}
              ++nsub_jet;
            }
          }
          //jet_electron_photon_e_frac /= jet_e;
        }

        // Generate a number of pileup events.
        //double mb_e(0); mb_electron_photon_e_frac(0);
        int nhits_bc(0);
        for (int iPileup = 0; iPileup < nPileup; ++iPileup) {

          if (!mb_pythia.next()) {
            if (++mb_iAbort < nAbort) continue;
            cout << " Event generation aborted prematurely, owing to error in minbias generation!\n";
            break;
          }

          double mb_weight = mb_pythia.info.mergingWeight();
          double mb_evtweight = mb_pythia.info.weight();
          mb_weight *= mb_evtweight;

          // Do not print zero-weight events.
          if ( mb_weight == 0. ) {
            --iPileup; continue;
          }

          // Find final charged particles with |eta| < etaMax
          det_acc_filter.filter( mb_pythia.event );

          // Further det_acc_filter particles to get only those within main event window
          std::vector< Particle* > minbias_within_window; std::vector< double > drvec;
          minbias_within_window.reserve( static_cast<int>( det_acc_filter.size() / 4) ); drvec.reserve( static_cast<int>( det_acc_filter.size() / 4) );
          for ( const auto p : det_acc_filter.getParticlesRef() ){
            for ( const auto &pj : pj_vec ) {
              const double deta = abs( p->eta() - pj.eta() );
              const double dphi = abs( phi( p->p(), pj ) );
              //std::cout << "dphi:" << dphi << "|phi1:" << p->p().phi() << "|phi2:" << pj.phi() << endl;
              if ( ( deta < mb_delta_eta ) &&
                   ( dphi < mb_delta_phi ) ) {
                // We found a particle to be kept, rotate it to jet axis
                minbias_within_window.push_back( p );
                //drvec.push_back( std::sqrt( deta*deta + dphi*dphi ) );
              }
            }
          }

          if ( minbias_within_window.empty() ) continue;

          const auto mb_event_t = mb_pythia.rndm.gauss() * sigma_t;
          const auto mb_event_z = mb_pythia.rndm.gauss() * sigma_z;

          // Fill minbias substruct
          std::size_t idx(0);
          for ( const auto& mb_p : minbias_within_window   ){
            // Shorthands
            const auto pdg_id = mb_p->id();
            // Obtain minbias data
            p_isMain->push_back( 0 );
            p_bc_id->push_back( bc_id );
            p_pdg_id->push_back( pdg_id );
            p_px->push_back( mb_p->px() ); 
            p_py->push_back( mb_p->py() ); 
            p_pz->push_back( mb_p->pz() );
            p_eta->push_back( mb_p->eta() ); 
            p_phi->push_back( mb_p->phi() );
            p_prod_x->push_back( mb_p->xProd() ); 
            p_prod_y->push_back( mb_p->yProd() ); 
            p_prod_z->push_back( mb_p->zProd() + mb_event_z ); 
            p_prod_t->push_back( mb_p->tProd() + mb_event_t  );
            p_e->push_back( mb_p->e() ); 
            p_et->push_back( mb_p->eT() );
            //mb_e += mb_p->e();
            //if ( pdg_id == 22 || abs(pdg_id) == 11 ){
            //  mb_electron_photon_e_frac += mb_e;
            //}
            //const double dR = drvec[idx];
            ++idx;
          }
          nhits_bc += minbias_within_window.size();
        } // Created all pile_up events for this BC

        //mb_electron_photon_e_frac_win += mb_electron_photon_e_frac;
        //mb_electron_photon_e_frac /= mb_e;
        //mb_e_win += mb_e;
        //const auto emb_perc = mb_e / jet_e;
        nhits_win += nhits_bc;

        // --- Fill bc_id branches
        bc_id_mu->push_back( nPileup ); 
        bc_id_nhits->push_back( nhits_bc );

        // --- 1D Minbias histograms
        th1_mu_bc.Fill( nPileup );
        th1_nhits_bc.Fill( nhits_bc );
      } // Finished all BCs

      //mb_electron_photon_e_frac_win /= mb_e_win;
      //const auto emb_perc_win = mb_e_win / jet_e;
      // Fill window specific information
      nPileUpMean /= nWin; avg_mu = nPileUpMean;

      // --- 1D Minbias histograms
      th1_avg_mu.Fill( avg_mu );
      th1_nhits_win.Fill( nhits_win );
      // --- 2D Minbias histograms

      // Fill main ttree
      tree.Fill();

      // Clear vectors
      p_isMain->clear();
      p_pdg_id->clear(); 
      p_bc_id->clear();
      p_px->clear(); 
      p_py->clear(); 
      p_pz->clear();
      p_eta->clear(); 
      p_phi->clear();
      p_prod_x->clear(); 
      p_prod_y->clear(); 
      p_prod_z->clear();  
      p_prod_t->clear();
      p_e->clear(); 
      p_et->clear();

    } catch ( NotInterestingEvent ){
      cout << "Ignoring non interesting event, regenerating..." << endl;
      --iEvent;
      continue;
    }

    // End of event loop.
  }

  // Final statistics.
  pythia.stat();
  mb_pythia.stat();

  tree.Write();

  file.Write();

  // Done.
  return 0;
}
