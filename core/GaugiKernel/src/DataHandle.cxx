
#include "GaugiKernel/DataHandle.h"



using namespace SG;


EventContext::EventContext( std::string name ): IMsgService(name)
{;}


EventContext::~EventContext()
{
  clear();
}



void EventContext::clear()
{
  m_storable_ptr.clear();
}







