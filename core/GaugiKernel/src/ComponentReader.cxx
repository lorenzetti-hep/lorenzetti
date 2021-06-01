
#include "GaugiKernel/ComponentReader.h"

using namespace Gaugi;


ComponentReader::ComponentReader(): 
  IMsgService(),
  PropertyService()
{;}



const std::string& ComponentReader::name() const
{
  return getLogName();
}



