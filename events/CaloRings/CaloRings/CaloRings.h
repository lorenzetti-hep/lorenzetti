#ifndef CaloRings_h
#define CaloRings_h

#include "CaloCluster/CaloCluster.h"
#include "CaloCell/CaloCell.h"
#include "GaugiKernel/EDM.h"
#include "GaugiKernel/macros.h"
#include <cmath>


// Event Object Data
namespace xAOD{
  
  class CaloRings: public Gaugi::EDM
  {  
    public:
      
      /*! Contructor */
      CaloRings();
      /*! Destructor */
      ~CaloRings()=default;
      /*! The eta center of the cluster given by the simulation (RoI) */ 
      PRIMITIVE_SETTER_AND_GETTER( float, m_eta, setEta, eta );
      /*! The phi center of the cluster given by the simulation (RoI) */ 
      PRIMITIVE_SETTER_AND_GETTER( float, m_phi, setPhi, phi );
      /*! Rings energy */
      PRIMITIVE_SETTER_AND_GETTER( std::vector<float> , m_rings, setRings, rings );
      /*! Set the associated CaloCluster to this CaloRings */
      void setCaloCluster( const xAOD::CaloCluster *clus ){ m_caloCluster=clus; };
      /*! Get the associated cluster to this CaloRings */
      const xAOD::CaloCluster* caloCluster() const { return m_caloCluster; };


    private:

      // Rings shape
      std::vector<float> m_rings;
      // CaloCluster used to build the rings
      const xAOD::CaloCluster *m_caloCluster;
      
      float m_eta;
      float m_phi;
  };


  

}
#endif
