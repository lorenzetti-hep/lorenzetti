
#include "GaugiKernel/Algorithm.h"

using namespace Gaugi;


Algorithm::Algorithm( std::string name ): 
  IMsgService(name),
  PropertyService(),
  m_isInitialized(false),
  m_isFinalized(false)
{;}

void Algorithm::setStoreGateSvc( SG::StoreGate *store )
{
  m_store = store;
}

SG::StoreGate* Algorithm::getStoreGateSvc() const
{
  return m_store;
}

const std::string& Algorithm::name() const
{
  return getLogName();
}

bool Algorithm::isFinalized()  const
{
  return m_isFinalized;
}

bool Algorithm::isInitialized() const
{
  return m_isInitialized;
}

void Algorithm::initLock()
{
  m_isInitialized=true;
}

void Algorithm::finaLock()
{
  m_isFinalized=true;
}





