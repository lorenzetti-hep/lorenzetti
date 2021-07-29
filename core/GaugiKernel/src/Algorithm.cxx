
#include "GaugiKernel/Algorithm.h"

using namespace Gaugi;


Algorithm::Algorithm(): 
  IMsgService(),
  PropertyService(),
  m_isInitialized(false),
  m_isFinalized(false)
{;}

//!=====================================================================

const std::string& Algorithm::name() const
{
  return getLogName();
}

//!=====================================================================

void Algorithm::initLock()
{
  m_isInitialized=true;
}

//!=====================================================================

void Algorithm::finaLock()
{
  m_isFinalized=true;
}

//!=====================================================================

bool Algorithm::isInitialized() const
{
  return m_isInitialized;
}

//!=====================================================================

bool Algorithm::isFinalized() const
{
  return m_isFinalized;
}
