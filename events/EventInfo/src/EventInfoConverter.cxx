
#include "EventInfo/EventInfoConverter.h"

using namespace xAOD;


bool EventInfoConverter::convert( const EventInfo *event, EventInfo_t &event_t) const
{
  if(event){
    event_t.runNumber   = event->runNumber();
    event_t.eventNumber = event->eventNumber();
    event_t.avgmu       = event->avgmu();
    return true;
  }
  return false;
}

bool EventInfoConverter::convert(const EventInfo_t &event_t, EventInfo *&event ) const
{
  event = new xAOD::EventInfo();
  event->setRunNumber( event_t.runNumber );
  event->setEventNumber( event_t.eventNumber );
  event->setAvgmu( event_t.avgmu);
  return true;
}

