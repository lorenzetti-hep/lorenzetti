#ifndef EventInfo_h
#define EventInfo_h

/** simulator libs **/
#include "GaugiKernel/EDM.h"
#include "GaugiKernel/macros.h"

/** geant 4 libs **/
#include "globals.hh"

/** standard libs **/
#include <vector>


namespace xAOD{

  class EventInfo: public Gaugi::EDM
  {  
    public:

      /** Constructor **/
      EventInfo();
      /** Destructor **/
      ~EventInfo();

      /** Average mu from the pythia generator **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_avgmu, setAvgmu, avgmu );
      /** Event number form geant4 **/
      PRIMITIVE_SETTER_AND_GETTER( int, m_eventNumber, setEventNumber, eventNumber );
      /** The event total energy **/
      PRIMITIVE_SETTER_AND_GETTER( int, m_totalEnergy, setTotalEnergy, totalEnergy );


    private:
      
      float m_eventNumber;
      float m_totalEnergy;
      float m_avgmu;
      
  };
}
#endif
