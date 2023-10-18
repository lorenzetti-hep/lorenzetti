#ifndef EventSeed_h
#define EventSeed_h

#include "GaugiKernel/EDM.h"
#include "GaugiKernel/macros.h"

namespace xAOD{
  
  class EventSeed: public Gaugi::EDM
  {  
    public:

      /*! Contructor */
      EventSeed();
      /*! Contructor */
      EventSeed( int id, float e, float et, float eta, float phi );
      /*! Destructor */
      ~EventSeed()=default;
      /*! The eta center of the cluster given by the simulation (RoI) */ 
      PRIMITIVE_SETTER_AND_GETTER( float, m_eta, setEta, eta );
      /*! The phi center of the cluster given by the simulation (RoI) */ 
      PRIMITIVE_SETTER_AND_GETTER( float, m_phi, setPhi, phi );
      /*! set/get energy */
      PRIMITIVE_SETTER_AND_GETTER( float, m_e, setE, e );
      /*! set/get transverse energy */
      PRIMITIVE_SETTER_AND_GETTER( float, m_et, setEt, et );
      
      PRIMITIVE_SETTER_AND_GETTER( int, m_id, setId, id );



    private:

      int   m_id;
      float m_e;
      float m_et;
      float m_eta;
      float m_phi;
  };
}
#endif
