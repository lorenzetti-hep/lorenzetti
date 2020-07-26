
#include "Zee.h"
#include "P8Kernel/ParticleFilter.h"
#include "P8Kernel/Event.h"

using namespace Pythia8;
using namespace generator;

Zee::Zee():
  IMsgService("Zee"),
  Algorithm()
{
  declareProperty( "File"           , m_file=""                   );
  declareProperty( "EtaMax"         , m_etaMax=1.4                );
  declareProperty( "MinPt"          , m_minPt=0.0                 );
}


Zee::~Zee()
{;}


StatusCode Zee::initialize()
{
  std::stringstream cmdseed; cmdseed << "Random:seed = " << m_seed;
  // Read in commands from external file.
  m_generator.readFile( m_file );
  m_generator.readString("Random:setSeed = on");
  m_generator.readString(cmdseed.str());
  m_nAbort = m_generator.mode("Main:timesAllowErrors");
  // Initialization for main (LHC) event
  m_generator.init();

  return StatusCode::SUCCESS;
}



StatusCode Zee::execute( generator::Event &event )
{

  // Generate main event. Quit if too many failures.
  if (!m_generator.next()) {
    if (m_iAbort++>m_nAbort){
      MSG_ERROR("Event generation aborted prematurely, owing to error in main event!" );
      throw AbortPrematurely();
    }
  }
  
  const auto main_event_t = sample_t();
  const auto main_event_z = sample_z();


  double weight = m_generator.info.mergingWeight();
  double evtweight = m_generator.info.weight();
  weight *= evtweight;

  // Do not print zero-weight events.
  if ( weight == 0. ) {
    return StatusCode::FAILURE;
  }

  std::vector<Particle*> zee; 
  zee.reserve(2);
  
  for (int i = 0; i < m_generator.event.size(); ++i) 
  {
    auto *p = &m_generator.event[i];

    if ( p->id() == 23 ){
      // We got our Z!
      for ( const auto cid : p->daughterListRecursive() ){
        auto *pchild = &m_generator.event[cid];
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

  // Each electron is a seed
  for ( auto& e : zee ){

    auto seed = generator::Seed( e->eta(), e->phi() );

    seed.emplace_back( 1, 0, e->id(), e->px(), e->py(), e->pz(), e->eta(), e->phi(), 
                       e->xProd(), e->yProd(), e->zProd() + main_event_z, e->tProd() + main_event_t, 
                       e->e(), e->eT() ); 

    event.push_back( seed );
  }

  
  return StatusCode::SUCCESS;
}



StatusCode Zee::finalize()
{
  MSG_INFO( "Finalize the Zee Event." );
  m_generator.stat();
  return StatusCode::SUCCESS;
}


