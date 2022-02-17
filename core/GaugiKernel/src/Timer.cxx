
#include "GaugiKernel/Timer.h"

namespace Gaugi{

  Timer::Timer()
  {
    start();
  }

  time_t Timer::resume(){
    update();
    return (m_end - m_start);
  }

  void Timer::start(){
    m_start = m_end = time(nullptr);
  }

  void Timer::stop(){
    m_end = time(nullptr);
  }

  void Timer::update(){
    m_end = time(nullptr);
  }

  void Timer::reset(){
    m_start = m_end = 0;
  }

}// namespace

