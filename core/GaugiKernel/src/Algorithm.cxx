
#include "GaugiKernel/Algorithm.h"

using namespace Gaugi;


Algorithm::Algorithm(): 
  IMsgService(),
  PropertyService()
{;}



const std::string& Algorithm::name() const
{
  return getLogName();
}



