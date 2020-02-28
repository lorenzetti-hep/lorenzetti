#ifndef EventInfo_h
#define EventInfo_h

/** simulator libs **/
#include "core/macros.h"

/** geant 4 libs **/
#include "globals.hh"

/** standard libs **/
#include <vector>


namespace xAOD{

  
  /// Description of a calorimeter cluster
  ///
  /// This is a first stab at how a calorimeter cell could behave in the
  /// xAOD EDM.
  /// @author Joao victor da Fonseca Pinto <jodafons@cern.ch, jodafons@lps.ufrj.br>
  ///
	/// $Revision: 1 $
  /// $Date: 2020-02-01$
  ///
  class EventInfo
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
      
      // event number
      float m_eventNumber;
      // event total energy
      float m_totalEnergy;
      // average mu
      float m_avgmu;
      
  };
}
#endif
