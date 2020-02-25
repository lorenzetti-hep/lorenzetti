
#include "core/AlgTool.h"


AlgTool::AlgTool( std::string &name ):
  IAlgTool(),
  m_name(name)
{;}

AlgTool::~AlgTool()
{;}


std::string AlgTool::name()
{
  return m_name;
}


void AlgTool::setStoreGateSvc( SG::StoreGate *store )
{
  m_store=store;
}

void AlgTool::setContext( EventContext *ctx )
{
  m_context = ctx;
}


EventContext* AlgTool::getContext()
{
  return m_context;
}


SG::StoreGate* AlgTool::getStoreGateSvc()
{
  return m_store;
}








