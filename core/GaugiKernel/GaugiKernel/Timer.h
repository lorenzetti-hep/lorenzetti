#ifndef Timer_h
#define Timer_h

#include <time.h>

namespace Gaugi{

  class Timer {
  
    public:
    
        Timer();
        ~Timer()=default;
       
        /*! Get the diff time */
        time_t resume();
    
        /*! Start clock */
        void start();
    
        /*! Stop clock */
        void stop();
    
        /*! Reset the clock */
        void reset();

        /*! Update the clock */
        void update();

    private:
      time_t m_start, m_end;
  };

}// namespace
#endif
