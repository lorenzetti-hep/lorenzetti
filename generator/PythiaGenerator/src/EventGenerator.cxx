
#include <algorithm>
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "EventGenerator.h"
#include "G4Kernel/CaloPhiRange.h"

static const double c_light = 2.99792458e+8; // m/s
using namespace Pythia8;
using namespace SG;




EventGenerator::EventGenerator():
  IMsgService("EventGenerator"),
  PropertyService(),
  m_store(nullptr)
{
  declareProperty( "NumberOfEvents" , m_nEvent=1                );
  declareProperty( "OutputFile"     , m_outputFile="particles"  );
  declareProperty( "OutputLevel"    , m_outputLevel=1           );
 
  /* Configure the pileup generation */
  declareProperty( "MinbiasFile"    , m_minbiasFile=""          );
  declareProperty( "Seed"           , m_seed=0/* clock system */);
  declareProperty( "PileupAvg"      , m_nPileupAvg=0            );
  declareProperty( "BunchIdStart"   , m_bc_id_start=-8          );
  declareProperty( "BunchIdEnd"     , m_bc_id_end=7             );
  declareProperty( "EtaMax"         , m_etaMax=1.4              );
  declareProperty( "Select"         , m_select=2                );
  declareProperty( "Sigma_t"        , m_sigma_t= 200 * 1e-12 /*pico seconds*/ * c_light /*m/s*/ * 1e+3 /*mm*/ );
  declareProperty( "Sigma_z"        , m_sigma_z=56 /*miliseconds*/                                            );
  declareProperty( "MinbiasDeltaEta", m_mb_delta_eta=0.22                                                     );
  declareProperty( "MinbiasDeltaPhi", m_mb_delta_phi=0.22                                                     );
  declareProperty( "UseWindow"      , m_useWindow=true                                                        );
  


}


EventGenerator::~EventGenerator()
{}



StatusCode EventGenerator::initialize()
{
  MSG_INFO( "Initialize pythia..." );
  
  setMsgLevel( m_outputLevel );

  m_store = new SG::StoreGate( m_outputFile);
  
  std::stringstream cmdseed; cmdseed << "Random:seed = " << m_seed;
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


  for( auto &tool : m_tools ){
    if ( tool->initialize().isFailure() ){
      MSG_FATAL( "It's not possible to initialize the event tool." );
    }
  }

  return StatusCode::SUCCESS;
}


StatusCode EventGenerator::run() 
{
  for (int iEvent = 0; iEvent < m_nEvent; ++iEvent) {
    
    MSG_INFO( "Running event " << iEvent );
    clear();
    try {
      std::vector<xAOD::seed_t> seed_vec;

      // Loop over all physcis tools
      for (auto& tool : m_tools ){
        if ( generate( tool, seed_vec ).isFailure() ){
          --iEvent; continue;
        }
      }

      addPileup( seed_vec );      
      // Fill main ttree
      m_tree->Fill();

    } catch ( NotInterestingEvent ){
      MSG_WARNING("Ignoring non interesting event, regenerating...");
      --iEvent;
      continue;
    } catch ( AbortPrematurely ){
      MSG_ERROR("Abort prematurely");
      break;
    }
  }// Loop over events

  return StatusCode::SUCCESS;
}



StatusCode EventGenerator::finalize()
{
  MSG_INFO( "Finalize the Event generator." );
  m_mb_pythia.stat();
  for( auto &tool : m_tools ){
    if ( tool->finalize().isFailure() ){
      MSG_ERROR( "It's not possible to finalize the event tool." );
    }
  }
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


StatusCode EventGenerator::addPileup( std::vector<xAOD::seed_t> seed_vec )
{

  ParticleFilter det_acc_filter( m_select, m_etaMax + .05, 0.7, 0.05 );
  const int nWin = m_bc_id_end - m_bc_id_start + 1;
  int mb_iAbort=0;

  double nPileUpMean(0);
  int nhits_win(0);
  
  for ( int bc_id = m_bc_id_start; bc_id <= m_bc_id_end; ++bc_id ){
    /*
     * MinBias generation
     */

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
      std::vector< Particle* > minbias_vec; 
      minbias_vec.reserve( static_cast<int>( det_acc_filter.size() / 4) ); 
      
      for ( const auto p : det_acc_filter.getParticlesRef() ){
        
        if (m_useWindow){ 
          for ( const auto &seed : seed_vec ) {
            const double deta = abs( p->eta() - seed.eta );
            const double dphi = abs( CaloPhiRange::diff( p->phi(), seed.phi ) );
            if ( ( deta < m_mb_delta_eta ) &&
                 ( dphi < m_mb_delta_phi ) ) {
              // We found a particle to be kept, rotate it to jet axis
              minbias_vec.push_back( p );
            }
          }
        }else{
          minbias_vec.push_back( p );

        }


      }

      if ( minbias_vec.empty() ) continue;

      const auto mb_event_t = m_mb_pythia.rndm.gauss() * m_sigma_t;
      const auto mb_event_z = m_mb_pythia.rndm.gauss() * m_sigma_z;

      // Fill minbias substruct
      std::size_t idx(0);
      for ( const auto& mb_p : minbias_vec   ){
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
      nhits_bc += minbias_vec.size();
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

  return StatusCode::SUCCESS;
}


StatusCode EventGenerator::generate( Physics *tool,  std::vector<xAOD::seed_t> &seed_vec )
{

  std::vector<std::vector<Particle*>> particles_vec;
  std::vector<xAOD::seed_t> seeds;


  // This can raise an NotInterestingEvent or AbortPrematurely or failure status code
  if( tool->run( seeds, particles_vec ).isFailure() )
  {
    return StatusCode::FAILURE;
  }

  // Just use the mb pythia to get random numbers
  const auto main_event_t = m_mb_pythia.rndm.gauss() * m_sigma_t;
  const auto main_event_z = m_mb_pythia.rndm.gauss() * m_sigma_z;


  // Fill the seed
  // Main event is always in BC 0; pdg is = 0 to sign that this is a RoI
  for ( std::size_t main_idx = 0; main_idx < particles_vec.size(); ++ main_idx ) {
    
    /*
     * Add seed position into the ntuple
     */

    // Set the main event just for location in Geant reconstruction
    const auto seed = seeds[main_idx];
    // Add this to be seed
    seed_vec.push_back(seed);

    m_p_isMain->push_back( 1 );
    m_p_bc_id->push_back( 0 ); 
    m_p_pdg_id->push_back( 0 );
    m_p_px->push_back( seed.px ); 
    m_p_py->push_back( seed.py ); 
    m_p_pz->push_back( seed.pz );
    m_p_eta->push_back( seed.eta ); 
    m_p_phi->push_back( seed.phi );
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
    
    m_store->hist1("eta")->Fill( seed.eta );
    m_store->hist1("phi")->Fill( seed.phi );
    m_store->hist1("pt")->Fill( ettot  );


    /*
     * Add all main particles into the ntuple
     */
    for ( const auto& part : particles ){
      // Shorthands
      const auto pdg_id = part->id();
      // Obtain substruct data
      m_p_isMain->push_back( 1 );
      m_p_bc_id->push_back( 0 ); 
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

  return StatusCode::SUCCESS;
}


