
#include "core/EventContext.h"

EventContext::EventContext()
{}


EventContext::~EventContext(){
  // Just to be sure to remove all allocated memory
  finalize();
}



StatusCode EventContext::initialize()
{
  return ErrorCode::SUCCESS;
}



StatusCode EventContext::finalize()
{
  //for ( auto& pairObj : m_stepHandleKey )
  //  delete pairObj.second;
  m_stepHandleKey.clear();

  for ( auto& pairObj : m_caloCellHandleKey )
    if(pairObj.second)  delete pairObj.second;
  m_caloCellHandleKey.clear();

  for ( auto& pairObj : m_caloClusterHandleKey )
    if(pairObj.second)  delete pairObj.second;
  m_caloClusterHandleKey.clear();
  
  for ( auto& pairObj : m_eventInfoHandleKey )
    if(pairObj.second)  delete pairObj.second;
  m_eventInfoHandleKey.clear();

  for ( auto& pairObj : m_truthHandleKey )
    if(pairObj.second)  delete pairObj.second;
  m_truthHandleKey.clear();
  
  return ErrorCode::SUCCESS;
}

