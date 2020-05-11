#include "Zee.h"
#include "EventInfo/EventInfo.h"

using namespace Pythia8;


Zee::Zee():
  IMsgService("Zee"),
  Physics(),
  m_abort(0)
{
  declareProperty( "MainFile"       , m_mainFile=""               );
  declareProperty( "EtaMax"         , m_etaMax=1.4                );
  declareProperty( "MinPt"          , m_minPt=0.0                 );
  declareProperty( "Seed"           , m_seed=0 /* clock system */ );
  declareProperty( "OutputLevel"    , m_outputLevel=1             );
}



StatusCode Zee::initialize()
{
  std::stringstream cmdseed; cmdseed << "Random:seed = " << m_seed;
  // Read in commands from external file.
  m_pythia.readFile( m_mainFile );
  m_pythia.readString("Random:setSeed = on");
  m_pythia.readString(cmdseed.str());
  m_nAbort = m_pythia.mode("Main:timesAllowErrors");
  // Initialization for main (LHC) event
  m_pythia.init();

  return StatusCode::SUCCESS;
}



StatusCode Zee::run( std::vector<xAOD::seed_t> &seed_vec,  std::vector<std::vector<Particle*>> &particles )
{


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

  auto event = m_pythia.event;
  // Do not print zero-weight events.
  if ( weight == 0. ) {
    return StatusCode::FAILURE;
  }

  std::vector<Particle*> zee; 
  zee.reserve(2);
  
  for (int i = 0; i < event.size(); ++i) {
    auto *p = &event[i];
    if ( p->id() == 23 ){
      // We got our Z!
      for ( const auto cid : p->daughterListRecursive() ){
        auto *pchild = &event[cid];
        if ( pchild->isFinal() ) {
          if ( pchild->idAbs() == 11 ){
            // Get Zee children
            if ( abs(pchild->eta()) < m_etaMax && pchild->pT() > (m_minPt/1.e3) ){
              zee.push_back( pchild );
            }
          } else {
            MSG_DEBUG("Event is not a Zee decay");
            MSG_DEBUG(" === final Z decay === pdg_id:" << pchild->id() << "|eta:" << abs(pchild->eta()) << "|et:" << pchild->pT() );
            throw NotInterestingEvent();
          }
        }
      }
      break;
    }
  }


  if ( zee.empty() ){
    MSG_DEBUG( "There is not Zee event inside of this event");
    throw NotInterestingEvent();
  }


  for ( auto& e : zee ){
    std::vector<Particle*> e_vec; e_vec.push_back(e);
    particles.push_back( e_vec );
    seed_vec.push_back( xAOD::seed_t{ (float)e->eT(), 
                                      (float)e->eta(), 
                                      (float)e->phi(), 
                                      (float)e->px(), 
                                      (float)e->py(), 
                                      (float)e->pz(), 
                                      e->id() } );
  }

  
  return StatusCode::SUCCESS;
}



StatusCode Zee::finalize()
{
  MSG_INFO( "Finalize the Zee Event." );
  m_pythia.stat();
  return StatusCode::SUCCESS;
}


