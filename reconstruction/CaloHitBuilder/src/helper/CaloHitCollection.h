/*
 * This is a special fast container to hold all hits.
 */


#ifndef CaloHitCollection_h
#define CaloHitCollection_h

#include "GaugiKernel/DataHandle.h"
#include "CaloHit/CaloHit.h"
#include "TVector3.h"
#include <memory>
#include <string>
#include <map>


namespace xAOD{

  class CaloHitCollection : public SG::DataHandle
  {  
    typedef std::map<unsigned int, xAOD::CaloHit* > collection_map_t;

    public:

      /*! Contructor */
      CaloHitCollection(  float etamin, float etamax,
                          std::vector<float> etabins,
                          std::vector<float> phibins,
                          float rmin,   float rmax, 
                          Detector detector,
                          CaloSampling sampling,
                          unsigned segmentation);
 
      /*! Destructor */
      ~CaloHitCollection();
      
      /*! Add a new calo cell into the collection */
      void push_back( xAOD::CaloHit* );
      /*! Zeroize all calo cells */
      void clear();
      /*! Return the number of cells into this collection */
      size_t size() const;
      /*! Retreive the correct cell given the step position */
      bool retrieve( TVector3 &, xAOD::CaloHit*& ) const;
      /*! Get the cell map */ 
      const collection_map_t& operator*() const;
      /*! Sampling */
      CaloSampling sampling() const;
    
    private:

      int findIndex( const std::vector<float> &vec, float value) const; 

      /*! Cell map into strings to faster access */
      collection_map_t  m_collection;
      /*! eta bins inside of this collection */
      std::vector<float> m_eta_bins;
      /*! phi bins inside of this collection */
      std::vector<float> m_phi_bins;
      /*! In plan xy */
      float m_radius_min;
      /*! In plan xy */
      float m_radius_max;

      /*! Segmentation id */
      unsigned m_segmentation;
      /*! Calorimeter sampling id */
      CaloSampling m_sampling;
      /*! Calorimeter detector */
      Detector m_detector;

      // eta bounds for this collection
      float m_etamin, m_etamax;
  };
}// namespace
#endif
