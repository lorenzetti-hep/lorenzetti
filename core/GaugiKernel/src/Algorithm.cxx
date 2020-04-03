
#include "GaugiKernel/Algorithm.h"


using namespace Gaugi;




Algorithm::Algorithm( std::string &name, MSG::Level level ): 
  IMsgService(name),
  MsgService(level),
  m_name(name)
{;}


  

void Algorithm::setStoreGateSvc( SG::StoreGate *store )
{
  m_store = store;
}


SG::StoreGate* Algorithm::getStoreGateSvc()
{
  return m_store;
}

std::string Algorithm::name()
{
  return m_name;
}


