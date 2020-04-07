#ifndef CaloRings_h
#define CaloRings_h

#include "CaloCluster/CaloCluster.h"
#include "CaloCell/CaloCell.h"
#include "GaugiKernel/macros.h"
#include <cmath>


// Event Object Data
namespace xAOD{
  
  class CaloRings
  {  
    public:
      
      /*! Contructor */
      CaloRings():m_caloCluster(nullptr) {};
      /*! Destructor */
      ~CaloRings()=default;

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

  };


  
  class RingSet{
  
    public:
      /*! Contructor */
      RingSet( CaloSampling::CaloSample , unsigned nrings, float deta, float dphi );
      /*! Destructor */
      RingSet()=default;
      /*! Add the cell energy to the correct ring position in this RingSet */
      void add( const xAOD::CaloCell *, float eta_center, float phi_center , bool truth=false);
      /*! Get the ringer shaper pattern for this RingSet */
      const std::vector<float>& pattern() const;
      /*! The number of rings in this RingSet */
      size_t size() const;
      /*! Sampling id */
      CaloSampling::CaloSample sampling() const;
      /*! Zeroize all energy values */
      void clear();


    private:

      std::vector<float> m_pattern;
      /*! Delta eta */
      float m_deta;
      /*! Delta phi */
      float m_dphi;
      /*! Sampling layer */
      CaloSampling::CaloSample m_sampling;
  };


}
#endif
