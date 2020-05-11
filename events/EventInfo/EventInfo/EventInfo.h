#ifndef EventInfo_h
#define EventInfo_h

/** simulator libs **/
#include "GaugiKernel/macros.h"

/** geant 4 libs **/
#include "globals.hh"

/** standard libs **/
#include <vector>


namespace xAOD{


  struct seed_t{ float et; float eta; float phi; float px; float py; float pz; int pdgid; };
 

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


      void clear();

      void push_back( seed_t seed );

      std::vector<seed_t> allSeeds() const ;

      size_t size() { return m_seed.size(); };


    private:
      
      float m_eventNumber;
      float m_totalEnergy;
      float m_avgmu;
      std::vector<seed_t> m_seed;
      
  };
}
#endif
