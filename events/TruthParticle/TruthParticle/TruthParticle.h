#ifndef TruthParticle_h
#define TruthParticle_h

#include "CaloCluster/CaloCluster.h"
#include "GaugiKernel/macros.h"

namespace xAOD{
  
  class TruthParticle
  {  
    public:

      /*! Contructor */
      TruthParticle();
      /*! Contructor */
      TruthParticle( float et, float eta, float phi, int pdgid );
      /*! Destructor */
      ~TruthParticle()=default;
      /*! The eta center of the cluster given by the simulation (RoI) */ 
      PRIMITIVE_SETTER_AND_GETTER( float, m_eta, setEta, eta );
      /*! The phi center of the cluster given by the simulation (RoI) */ 
      PRIMITIVE_SETTER_AND_GETTER( float, m_phi, setPhi, phi );
      /*! set/get transverse energy */
      PRIMITIVE_SETTER_AND_GETTER( float, m_et, setEt, et );
      /*! set/get particle identification */
      PRIMITIVE_SETTER_AND_GETTER( int, m_pdgid, setPdgid, pdgid );
      /*! Set the associatet cluster */
      void setCaloCluster( const xAOD::CaloCluster *clus ){ m_caloCluster=clus; };
      /*! Get the associatet cluster */
      const xAOD::CaloCluster* caloCluster() const { return m_caloCluster; };

    private:

      int m_pdgid;
      float m_et;
      float m_eta;
      float m_phi;
      const xAOD::CaloCluster *m_caloCluster;

  };
}
#endif
