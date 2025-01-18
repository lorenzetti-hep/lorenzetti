
#include "GaugiKernel/DataHandle.h"





using namespace SG;


EventContext::EventContext( std::string name ): 
  IMsgService(name),
  m_threadId(0)
{;}


EventContext::~EventContext()
{
  clear();
}



void EventContext::clear()
{
  //for (auto &it : m_storable_ptr)
  //  std::cout << it.first << std::endl;
  m_storable_ptr.clear();
}







