#include "GaugiKernel/MsgStream.h"

constexpr unsigned MsgStreamMirror::Message::space_between_log_and_msg;
constexpr const char* MsgStreamMirror::Message::color[];

//==============================================================================
MsgStreamMirror& MsgStreamMirror::doOutput() 
{
  try {
    // This piece of code may throw and we cannot afford it when we print a
    // message in the middle of a catch block.
    if ( isActive() )   {
      Message msg(m_streamName, m_currentLevel, m_useColor, m_stream.str());
//#if USE_OMP
//      #pragma omp critical
//#endif
      std::cout << msg << std::endl;
    }
    // Reset our stream
    m_stream.str("");
  } catch(...) {}
  return *this;
}

//==============================================================================
void MsgStreamMirror::print( const MSG::Level lvl )
{
  MSG::Level prev_lvl = m_currentLevel;
  m_stream << lvl << " stream with message level "
    << to_str(prev_lvl) << " and logname \"" 
    << m_streamName << "\"";
  doOutput();
}

