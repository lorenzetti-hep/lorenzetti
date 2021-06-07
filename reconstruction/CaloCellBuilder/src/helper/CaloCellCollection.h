/*
 * This is a special fast container to hold all hits.
 */


#ifndef CaloCellCollection_h
#define CaloCellCollection_h

#include "GaugiKernel/DataHandle.h"
#include "CaloCell/CaloDetDescriptor.h"
#include "CaloHit/CaloHit.h"
#include <memory>
#include <string>
#include <map>


namespace xAOD{

  class CaloCellCollection : public SG::DataHandle
  {  
    typedef std::map<unsigned long int, xAOD::CaloDetDescriptor* > collection_map_t;

    public:

      /*! Contructor */
      CaloCellCollection( float etamin, float etamax,
                          std::vector<float> etabins,
                          std::vector<float> phibins,
                          float rmin,   float rmax, 
                          Detector detector,
                          CaloSampling sampling,
                          unsigned segmentation);
 
      /*! Destructor */
      ~CaloCellCollection();
      
      /*! Add a new calo cell into the collection */
      void push_back( xAOD::CaloDetDescriptor* );
      /*! Zeroize all calo cells */
      void clear();
      /*! Return the number of cells into this collection */
      size_t size() const;
      /*! Retreive the correct cell given the step position */
      bool retrieve( const xAOD::CaloHit* , xAOD::CaloDetDescriptor*& ) const;
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
