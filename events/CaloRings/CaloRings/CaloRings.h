#ifndef CaloRings_h
#define CaloRings_h


#include "CaloCell/CaloCell.h"
#include "GaugiKernel/macros.h"



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
      RingSet( CaloSampling::CaloSample , unsigned nrings, float deta, float dphi );
      RingSet()=default;

      void add( const xAOD::CaloCell *, float eta_center, float phi_center , bool truth=false);

      const std::vector<float>& pattern() const;

      size_t size() const;
  

    private:

      std::vector<float> m_pattern;
      /*! Delta eta */
      float m_deta;
      /*! Delta phi */
      float m_dphi;
  };


  RingSet::RingSet( CaloSampling::CaloSample sampling, unsigned nrings, float deta, float dphi ):
    m_sampling(sampling), m_rings(nrings,0), m_deta(deta), m_dphi(dphi)
  {;}



  void RingSet::add( const xAOD::CaloCell *cell , float eta_center, float phi_center, bool truth)
  {
    // This cell does not allow to this RingSet
    if( cell->sampling() != m_sampling )  return;
    deta = std::abs( eta_center, cell->eta() ) / m_deta;
    dphi = std::abs( phi_center, cell->phi() ) / m_dphi;
    float deltaGreater = std::max(deta, dphi)
    int i = static_cast<unsigned int>( std::floor(deltaGreater) );
    
    if( i < m_pattern.size() ){
      m_pattern[i] += (truth? cell->truthRawEnergy() : cell->energy()  ) / std::cosh(std::abs(eta_center));
    }
  }


  size_t RingSet::size() const
  {
    return m_rings.size()
  }


  const std::vector<float>& RingSet::pattern() const
  {
    return m_pattern;
  }







  


}
#endif
