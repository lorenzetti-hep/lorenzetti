
#include "GaugiKernel/AlgTool.h"


using namespace Gaugi;




AlgTool::AlgTool( std::string &name ): 
  IMsgService(name)
{;}


  

void AlgTool::setStoreGateSvc( SG::StoreGate *store )
{
  m_store = store;
}


SG::StoreGate* AlgTool::getStoreGateSvc() const
{
  return m_store;
}


const std::string& AlgTool::name() const
{
  return getLogName();
}


