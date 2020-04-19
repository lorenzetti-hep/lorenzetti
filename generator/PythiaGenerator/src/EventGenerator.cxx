
#include <algorithm>
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "EventGenerator.h"


static const double c_light = 2.99792458e+8; // m/s
using namespace Pythia8;
using namespace SG;




EventGenerator::EventGenerator():
  IMsgService("EventGenerator"),
  PropertyService(),
  m_store(nullptr)
{
  declareProperty( "NumberOfEvents" , m_nEvent=1                );
  declareProperty( "MainFile"       , m_mainFile=""             );
  declareProperty( "MinbiasFile"    , m_minbiasFile=""          );
  declareProperty( "OutputFile"     , m_outputFile="particles"  );
  declareProperty( "EtaMax"         , m_etaMax=1.4              );
  declareProperty( "MinPt"          , m_minPt=0.0               );
  declareProperty( "PileupAvg"      , m_nPileupAvg=0            );
  declareProperty( "BunchIdStart"   , m_bc_id_start=-8          );
  declareProperty( "BunchIdEnd"     , m_bc_id_end=7             );
  declareProperty( "Select"         , m_select=2                );
  declareProperty( "Sigma_t"        , m_sigma_t= 200 * 1e-12 /*pico seconds*/ * c_light /*m/s*/ * 1e+3 /*mm*/ );
  declareProperty( "Sigma_z"        , m_sigma_z=56 /*miliseconds*/                                            );
  declareProperty( "MinbiasDeltaEta", m_mb_delta_eta=0.22                                                     );
  declareProperty( "MinbiasDeltaPhi", m_mb_delta_phi=0.22                                                     );
  declareProperty( "Seed"           , m_seed=0 /* clock system */                                             );
  declareProperty( "OutputLevel"    , m_outputLevel=1                                                         );
}


EventGenerator::~EventGenerator()
{}


StatusCode EventGenerator::initialize()
{
  setMsgLevel( m_outputLevel );

  MSG_INFO( "Initialize pythia..." );

  m_store = new SG::StoreGate( m_outputFile);
  
  m_minPt = m_minPt/1.e3;
  
  std::stringstream cmdseed; cmdseed << "Random:seed = " << m_seed;

  MSG_INFO( cmdseed.str() );

  // Read in commands from external file.
  m_pythia.readFile( m_mainFile );
  m_pythia.readString("Random:setSeed = on");
  m_pythia.readString(cmdseed.str());


  // Number of events to generate, to list, to allow aborts.
  //m_nEvent = m_pythia.mode("Main:numberOfEvents");
  m_nAbort = m_pythia.mode("Main:timesAllowErrors");
  // Initialization for main (LHC) event
  m_pythia.init();
  // Minbias generator
  m_mb_pythia.readFile( m_minbiasFile );
  m_mb_pythia.readString("Random:setSeed = on");
  m_mb_pythia.readString(cmdseed.str());
  m_mb_pythia.init();
  

  // Initialize output file
  m_tree = new TTree("particles","Pythia particles event tree");
  m_tree->Branch("avg_mu"      , &m_avg_mu, "avg_mu/F");
  m_tree->Branch("p_isMain"    , &m_p_isMain     );
  m_tree->Branch("p_pdg_id"    , &m_p_pdg_id     ); 
  m_tree->Branch("p_bc_id"     , &m_p_bc_id      ); 
  m_tree->Branch("bc_mu"       , &m_bc_id_mu     ); 
  m_tree->Branch("bc_id_nhits" , &m_bc_id_nhits  );
  m_tree->Branch("p_px"        , &m_p_px         ); 
  m_tree->Branch("p_py"        , &m_p_py         ); 
  m_tree->Branch("p_pz"        , &m_p_pz         );
  m_tree->Branch("p_prod_x"    , &m_p_prod_x     ); 
  m_tree->Branch("p_prod_y"    , &m_p_prod_y     ); 
  m_tree->Branch("p_prod_z"    , &m_p_prod_z     );  
  m_tree->Branch("p_prod_t"    , &m_p_prod_t     );
  m_tree->Branch("p_eta"       , &m_p_eta        ); 
  m_tree->Branch("p_phi"       , &m_p_phi        );
  m_tree->Branch("p_e"         , &m_p_e          );  
  m_tree->Branch("p_et"        , &m_p_et         );
  m_store->cd();
  m_store->add( m_tree );


  m_store->add( new TH1F( "avgmu", "<#mu>;<#mu>;Count", 100, 0, 100 ) );
  m_store->add( new TH1F( "eta"  , "#eta Main particles; #eta; Count", 50, -2.5, 2.5 ) );
  m_store->add( new TH1F( "phi"  , "#phi Main particles; #phi; Count", 50, -3.2, 3.2 ) );
  m_store->add( new TH1F( "pt"  , "P_{T} Main particles; P_{T}[GeV]; Count", 100, 0, 100 ) );




  return StatusCode::SUCCESS;
}






StatusCode EventGenerator::run() 
{

  const int nWin = m_bc_id_end - m_bc_id_start + 1;
  
  // Declare Event Filter according to specification. 
  // 700 MeV cut to remove charged particles trapped inside detector
  ParticleFilter det_acc_filter( m_select, m_etaMax + .05, 0.7, 0.05 );
 

  // Begin event loop.
  int iAbort = 0; int mb_iAbort = 0;
  for (int iEvent = 0; iEvent < m_nEvent; ++iEvent) {


    MSG_INFO( "Running event " << iEvent );
    // Generate main event. Quit if too many failures.
    if (!m_pythia.next()) {
      if (++iAbort < m_nAbort) continue;
      MSG_WARNING("Event generation aborted prematurely, owing to error in main event!" );
      break;
    }

    double weight = m_pythia.info.mergingWeight();
    double evtweight = m_pythia.info.weight();
    weight *= evtweight;

    // Do not print zero-weight events.
    if ( weight == 0. ) {
      --iEvent; continue;
    }

    try {

      std::vector<std::vector<Particle*>> particles_vec;
      std::vector<Particle*> main_particles_vec;

      generate( m_pythia.event, main_particles_vec, particles_vec );

      const auto main_event_t = m_pythia.rndm.gauss() * m_sigma_t;
      const auto main_event_z = m_pythia.rndm.gauss() * m_sigma_z;

      std::vector<Vec4> roi_vec;

      // Main event is always in BC 0; pdg is = 0 to sign that this is a RoI
      for ( std::size_t main_idx = 0; main_idx < particles_vec.size(); ++ main_idx ) {
        // Set the main event just for location in Geant reconstruction
        const auto p = main_particles_vec[main_idx];
        roi_vec.push_back( Vec4(p->px(), p->py(), p->pz() ));
        m_p_isMain->push_back( 1 );
        m_p_bc_id->push_back( 0 ); 
        m_p_pdg_id->push_back( 0 );
        m_p_px->push_back( p->px() ); 
        m_p_py->push_back( p->py() ); 
        m_p_pz->push_back( p->pz() );
        m_p_eta->push_back( p->eta() ); 
        m_p_phi->push_back( p->phi() );
        m_p_prod_x->push_back( 0 ); 
        m_p_prod_y->push_back( 0 ); 
        m_p_prod_z->push_back( 0 + main_event_z  ); 
        m_p_prod_t->push_back( 0 + main_event_t );
       
        // In case of cluster, sum the energy of all particles
        const auto particles = particles_vec[main_idx];
        float etot=0.0; float ettot=0.0;
        for( const auto& pj : particles ){ 
          etot+=pj->e(); ettot+=pj->eT();
        }

        m_p_e->push_back( etot ); 
        m_p_et->push_back( ettot );
        m_store->hist1("eta")->Fill( p->eta() );
        m_store->hist1("phi")->Fill( p->phi() );
        m_store->hist1("pt")->Fill( ettot );
 
      }

      // Retrieve minbias
      double nPileUpMean(0);
      int nhits_win(0);

      for ( int bc_id = m_bc_id_start; bc_id <= m_bc_id_end; ++bc_id ){
        // Generate main event. Quit if too many failures.


        // Fill main event particles
        if ( bc_id == 0 ) {
          // Jet variables/branches
          // Main event is always in BC 0; pdg is = 0 to sign that this is a RoI
          for ( std::size_t main_idx = 0; main_idx < particles_vec.size(); ++ main_idx ) {
            const auto particles = particles_vec[main_idx];
            // Fill substruct
            for ( const auto& part : particles ){
              // Shorthands
              const auto pdg_id = part->id();
              // Obtain substruct data
              m_p_isMain->push_back( 1 );
              m_p_bc_id->push_back( bc_id ); 
              m_p_pdg_id->push_back( pdg_id );
              m_p_px->push_back( part->px() ); 
              m_p_py->push_back( part->py() ); 
              m_p_pz->push_back( part->pz() );
              m_p_eta->push_back( part->eta() ); 
              m_p_phi->push_back( part->phi() );
              m_p_prod_x->push_back( part->xProd() ); 
              m_p_prod_y->push_back( part->yProd() ); 
              m_p_prod_z->push_back( part->zProd() + main_event_z  ); 
              m_p_prod_t->push_back( part->tProd() + main_event_t );
              m_p_e->push_back( part->e() ); 
              m_p_et->push_back( part->eT() );
              
           
            }
          }
        }

        // Select the number of pileup events to generate.
        int nPileup = poisson(m_nPileupAvg, m_mb_pythia.rndm);
        nPileUpMean += nPileup;

        int nhits_bc(0);
        // Generate a number of pileup events.
        for (int iPileup = 0; iPileup < nPileup; ++iPileup) {

          if (!m_mb_pythia.next()) {
            if (++mb_iAbort < m_nAbort) continue;
            MSG_WARNING("Event generation aborted prematurely, owing to error in minbias generation!");
            break;
          }

          double mb_weight = m_mb_pythia.info.mergingWeight();
          double mb_evtweight = m_mb_pythia.info.weight();
          mb_weight *= mb_evtweight;

          // Do not print zero-weight events.
          if ( mb_weight == 0. ) {
            --iPileup; continue;
          }

          // Find final charged particles with |eta| < etaMax
          det_acc_filter.filter( m_mb_pythia.event );

          // Further det_acc_filter particles to get only those within main event window
          std::vector< Particle* > minbias_within_window; 
          std::vector< double > drvec;
          minbias_within_window.reserve( static_cast<int>( det_acc_filter.size() / 4) ); 
          drvec.reserve( static_cast<int>( det_acc_filter.size() / 4) );
          
          for ( const auto p : det_acc_filter.getParticlesRef() ){
            for ( const auto &pj : roi_vec ) {
              const double deta = abs( p->eta() - pj.eta() );
              const double dphi = abs( phi( p->p(), pj ) );
              if ( ( deta < m_mb_delta_eta ) &&
                   ( dphi < m_mb_delta_phi ) ) {
                // We found a particle to be kept, rotate it to jet axis
                minbias_within_window.push_back( p );
                //drvec.push_back( std::sqrt( deta*deta + dphi*dphi ) );
              }
            }
          }

          if ( minbias_within_window.empty() ) continue;

          const auto mb_event_t = m_mb_pythia.rndm.gauss() * m_sigma_t;
          const auto mb_event_z = m_mb_pythia.rndm.gauss() * m_sigma_z;

          // Fill minbias substruct
          std::size_t idx(0);
          for ( const auto& mb_p : minbias_within_window   ){
            // Shorthands
            const auto pdg_id = mb_p->id();
            // Obtain minbias data
            m_p_isMain->push_back( 0 );
            m_p_bc_id->push_back( bc_id );
            m_p_pdg_id->push_back( pdg_id );
            m_p_px->push_back( mb_p->px() ); 
            m_p_py->push_back( mb_p->py() ); 
            m_p_pz->push_back( mb_p->pz() );
            m_p_eta->push_back( mb_p->eta() ); 
            m_p_phi->push_back( mb_p->phi() );
            m_p_prod_x->push_back( mb_p->xProd() ); 
            m_p_prod_y->push_back( mb_p->yProd() ); 
            m_p_prod_z->push_back( mb_p->zProd() + mb_event_z ); 
            m_p_prod_t->push_back( mb_p->tProd() + mb_event_t  );
            m_p_e->push_back( mb_p->e() ); 
            m_p_et->push_back( mb_p->eT() );
            ++idx;
          }
          nhits_bc += minbias_within_window.size();
        } // Created all pile_up events for this BC

        nhits_win += nhits_bc;

        // --- Fill bc_id branches
        m_bc_id_mu->push_back( nPileup ); 
        m_bc_id_nhits->push_back( nhits_bc );

      } // Finished all BCs

      //mb_electron_photon_e_frac_win /= mb_e_win;
      //const auto emb_perc_win = mb_e_win / jet_e;
      // Fill window specific information
      nPileUpMean /= nWin; m_avg_mu = nPileUpMean;
      m_store->hist1("avgmu")->Fill(m_avg_mu);

      // Fill main ttree
      m_tree->Fill();

    } catch ( NotInterestingEvent ){
      MSG_WARNING("Ignoring non interesting event, regenerating...");
      --iEvent;
      continue;
    }

    clear();
    // End of event loop.
  }

  return StatusCode::SUCCESS;
}



StatusCode EventGenerator::finalize()
{
  MSG_INFO( "Finalize the Event generator." );
  m_pythia.stat();
  m_mb_pythia.stat();
  delete m_store;
  return StatusCode::SUCCESS;
}




void EventGenerator::clear()
{
  // Clear vectors
  m_p_isMain->clear();
  m_p_pdg_id->clear(); 
  m_p_bc_id->clear();
  m_p_px->clear(); 
  m_p_py->clear(); 
  m_p_pz->clear();
  m_p_eta->clear(); 
  m_p_phi->clear();
  m_p_prod_x->clear(); 
  m_p_prod_y->clear(); 
  m_p_prod_z->clear();  
  m_p_prod_t->clear();
  m_p_e->clear(); 
  m_p_et->clear();
}




// Method to pick a number according to a Poissonian distribution.
int EventGenerator::poisson(double nAvg, Rndm& rndm) {
  // Set maximum to avoid overflow.
  const int nmax = 300;
  // Random number.
  double rPoisson = rndm.flat() * exp(nAvg);
  // Initialize.
  double rSum  = 0.;
  double rTerm = 1.;
  // Add to sum and check whether done.
  for (int i = 0; i < nmax; ) {
    rSum += rTerm;
    if (rSum > rPoisson) return i;
    // Evaluate next term.
    ++i;
    rTerm *= nAvg / i;
  }
  // Emergency return.
  return nmax;
}




