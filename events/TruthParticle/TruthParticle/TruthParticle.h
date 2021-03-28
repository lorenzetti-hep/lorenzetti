#ifndef TruthParticle_h
#define TruthParticle_h

#include "CaloCluster/CaloCluster.h"
#include "GaugiKernel/EDM.h"
#include "GaugiKernel/macros.h"

namespace xAOD{
  
  class TruthParticle: public Gaugi::EDM
  {  
    public:

      /*! Contructor */
      TruthParticle();
      /*! Contructor */
      TruthParticle( float e, float et, float eta, float phi, int pdgid );
      /*! Destructor */
      ~TruthParticle()=default;
      /*! The eta center of the cluster given by the simulation (RoI) */ 
      PRIMITIVE_SETTER_AND_GETTER( float, m_eta, setEta, eta );
      /*! The phi center of the cluster given by the simulation (RoI) */ 
      PRIMITIVE_SETTER_AND_GETTER( float, m_phi, setPhi, phi );
      /*! set/get energy */
      PRIMITIVE_SETTER_AND_GETTER( float, m_e, setE, e );
      /*! set/get transverse energy */
      PRIMITIVE_SETTER_AND_GETTER( float, m_et, setEt, et );
      /*! set/get particle identification */
      PRIMITIVE_SETTER_AND_GETTER( int, m_pdgid, setPdgid, pdgid );
      

    private:

      int m_pdgid;
      float m_e;
      float m_et;
      float m_eta;
      float m_phi;
  };
}
#endif
