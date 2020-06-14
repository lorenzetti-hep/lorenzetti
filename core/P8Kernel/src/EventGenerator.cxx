

//#include <algorithm>
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
//#include "G4Kernel/CaloPhiRange.h"
#include "P8Kernel/EventGenerator.h"

using namespace SG;
using namespace Gaugi;
using namespace generator;


EventGenerator::EventGenerator():
  IMsgService("EventGenerator"),
  PropertyService(),
  m_store(nullptr)
{

  declareProperty( "NumberOfEvents" , m_nEvent=1                );
  declareProperty( "OutputFile"     , m_outputFile="particles"  );
  declareProperty( "OutputLevel"    , m_outputLevel=1           );
}


EventGenerator::~EventGenerator()
{}



StatusCode EventGenerator::initialize()
{
  MSG_INFO( "Initialize pythia..." );
 
  setMsgLevel( m_outputLevel );

  m_store = new SG::StoreGate( m_outputFile);
 

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


  // Create some monitoring histograms
  m_store->add( new TH1F( "avgmu", "<#mu>;<#mu>;Count", 100, 0, 100 ) );
  m_store->add( new TH1F( "eta"  , "#eta Main particles; #eta; Count", 50, -2.5, 2.5 ) );
  m_store->add( new TH1F( "phi"  , "#phi Main particles; #phi; Count", 50, -3.2, 3.2 ) );
  m_store->add( new TH1F( "pt"  , "P_{T} Main particles; P_{T}[GeV]; Count", 100, 0, 100 ) );

  for( auto &alg : m_algs ){
    if ( alg->initialize().isFailure() ){
      MSG_FATAL( "It's not possible to initialize the event algorithm with name " << alg->name() );
    }
  }

  return StatusCode::SUCCESS;
}




StatusCode EventGenerator::execute() 
{

  for (int iEvent = 0; iEvent < m_nEvent; ++iEvent) {
    
    MSG_INFO( "Running event " << iEvent );
    clear();
    
    try {  

      Event event;

      // Loop over all physcis tools
      for (auto& alg : m_algs ){
        if ( alg->execute(event).isFailure() )
        {
          --iEvent; continue;
        }
      }

      dump( event );      
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
  
  for( auto &alg : m_algs ){
    if ( alg->finalize().isFailure() ){
      MSG_ERROR( "It's not possible to finalize the event algorithm with name " << alg->name() );
    }
  }
  // Release the storegate
  delete m_store;

  return StatusCode::SUCCESS;
}



void EventGenerator::clear()
{
  // Clear the ntuple vectors
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


void EventGenerator::dump( Event &event )
{

  // Just use the mb pythia to get random numbers
  //const auto main_event_t = m_mb_pythia.rndm.gauss() * m_sigma_t;
  //const auto main_event_z = m_mb_pythia.rndm.gauss() * m_sigma_z;


  // Fill the seed
  // Main event is always in BC 0; pdg is = 0 to sign that this is a RoI
  for ( auto &seed : *event ) 
  {  
    // All cluster is a seed and should be pdg id equal zero
    m_p_isMain->push_back( 1 );
    m_p_bc_id->push_back( 0 ); 
    m_p_pdg_id->push_back( 0 ); // Indicate to g4kernel that this is a seed
    m_p_px->push_back( 0 ); 
    m_p_py->push_back( 0 ); 
    m_p_pz->push_back( 0 );
    m_p_eta->push_back( seed.eta() ); 
    m_p_phi->push_back( seed.phi() );
    m_p_prod_x->push_back( 0 ); 
    m_p_prod_y->push_back( 0 ); 
    m_p_prod_z->push_back( 0 ); 
    m_p_prod_t->push_back( 0 );
    m_p_e->push_back( seed.etot() ); 
    m_p_et->push_back( seed.ettot() );
    
    // Fill histograms
    m_store->hist1("eta")->Fill( seed.eta() );
    m_store->hist1("phi")->Fill( seed.phi() );
    m_store->hist1("pt")->Fill( seed.ettot()  );


    for ( const auto& part : *seed ){
      // Obtain substruct data
      m_p_isMain->push_back( part.isMain );
      m_p_bc_id->push_back( part.bc_id ); 
      m_p_pdg_id->push_back( part.pdg_id );
      m_p_px->push_back( part.px ); 
      m_p_py->push_back( part.py ); 
      m_p_pz->push_back( part.pz );
      m_p_eta->push_back( part.eta ); 
      m_p_phi->push_back( part.phi );
      m_p_prod_x->push_back( part.xProd ); 
      m_p_prod_y->push_back( part.yProd ); 
      m_p_prod_z->push_back( part.zProd );//+ main_event_z  ); 
      m_p_prod_t->push_back( part.tProd );//+ main_event_t );
      m_p_e->push_back( part.e ); 
      m_p_et->push_back( part.eT );
    }
  }

  
}

