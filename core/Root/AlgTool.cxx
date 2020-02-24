
#include "AlgTool.h"


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














