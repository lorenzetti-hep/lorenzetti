
#include "EventInfo/EventInfo.h"

using namespace xAOD;

EventInfo::EventInfo():
      m_eventNumber(0),
      m_totalEnergy(0),
      m_avgmu(0)
{;}


EventInfo::~EventInfo()
{;}


void EventInfo::clear()
{
  m_seed.clear();
}


void EventInfo::push_back( const seed_t seed )
{
  m_seed.push_back(seed);
}


std::vector<seed_t> EventInfo::seeds() const 
{
  return m_seed;
}




