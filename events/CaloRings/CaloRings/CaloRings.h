#ifndef CaloRings_h
#define CaloRings_h


/** simulator includes **/
#include "CaloCell/CaloCell.h"
#include "GaugiKernel/macros.h"

/** geant 4 includes **/
#include "globals.hh"

/** standard includes **/
#include <vector>


// Event Object Data
namespace xAOD{
  
  class CaloRings
  {  
    public:
      
      /*! Contructor */
      CaloRings()=default;
      /*! Destructor */
      ~CaloRings()=default;


   

    private:
      
  };


  
  class RingSet{
  
    public:
      RingSet( CaloSampling::CaloSample , unsigned nrings, float deta, float dphi );
      RingSet()=default;

      void push_back( const xAOD::CaloCell * );

      const std::vector<float>& rings() const;

      size_t size() const;
  

    private:

      std::vector<float> m_rings;
      /*! Delta eta */
      float m_deta;
      /*! Delta phi */
      float m_dphi;
  };


  RingSet::RingSet( CaloSampling::CaloSample sampling, unsigned nrings, float deta, float dphi ):
    m_sampling(sampling), m_rings(nrings,0), m_deta(deta), m_dphi(dphi)
  {;}



  void RingSet::add( const xAOD::CaloCell *cell , const float eta_center, const float phi_center, bool truth)
  {
    // This cell does not allow to this RingSet
    if( cell->sampling() != m_sampling )  return;
    deta = std::abs( eta_center, cell->eta() ) / m_deta;
    dphi = std::abs( phi_center, cell->phi() ) / m_dphi;
    float deltaGreater = std::max(deta, dphi)
    int i = static_cast<unsigned int>( std::floor(deltaGreater) );
    
    if( i < m_rings.size() ){
      m_rings[i] += cell->energy() / std::cosh(std::abs(eta_center));
    }
  }


  size_t RingSet::size() const
  {
    return m_rings.size()
  }


  const std::vector<float>& RingSet::rings() const
  {
    return m_rings;
  }







  


}
#endif
