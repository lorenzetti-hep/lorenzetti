
#include "EventInfo/EventInfoConverter.h"
//#include "G4Kernel/macros.h"


using namespace xAOD;



bool EventInfoConverter::convert( const EventInfo *event, EventInfo_t &event_t)
{
  if(event){
    event_t.eventNumber = event->eventNumber();
    event_t.avgmu = event->avgmu();
    return true;
  }
  return false;
}

bool EventInfoConverter::convert(const EventInfo_t &event_t, EventInfo *&event )
{
  event = new xAOD::EventInfo();
  event->setEventNumber( event_t.eventNumber );
  event->setAvgmu( event_t.avgmu);
  return true;
}