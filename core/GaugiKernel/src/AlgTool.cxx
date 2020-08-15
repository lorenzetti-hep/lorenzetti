
#include "GaugiKernel/AlgTool.h"


using namespace Gaugi;




AlgTool::AlgTool(): 
  IMsgService()
{;}


  

const std::string& AlgTool::name() const
{
  return getLogName();
}


