#include "Zee.h"

using namespace Pythia8;


Zee::Zee():
  IMsgService("Zee"),
  EventGenerator()
{}



StatusCode Zee::generate( Event& event, std::vector<Particle*> &interest_particles, std::vector<std::vector<Particle*>>& particles )
{

  particles.clear();
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
            if ( abs(pchild->eta()) < m_etaMax && pchild->pT() > m_minPt ){
              zee.push_back( pchild );
              interest_particles.push_back( pchild );
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
  particles.push_back( zee );

  return StatusCode::SUCCESS;
}


