#ifndef CaloCellCollection_h
#define CaloCellCollection_h

#include "GaugiKernel/DataHandle.h"
#include "CaloCell/RawCell.h"
#include "TVector3.h"
#include <memory>
#include <string>
#include <map>


namespace xAOD{

  class CaloCellCollection : public SG::DataHandle
  {  
    typedef std::map<std::string, xAOD::RawCell* > collection_map_t;

    public:

      /*! Contructor */
      CaloCellCollection( float etamin, float etamax, float etabins, float phimin, float phimax, 
                          float phibins,float rmin,   float rmax, CaloSampling::CaloSample sampling);
 
      /*! Destructor */
      ~CaloCellCollection();
      /*! Add a new calo cell into the collection */
      void push_back( xAOD::RawCell* );
      /*! Zeroize all calo cells */
      void clear();
      /*! Return the number of cells into this collection */
      size_t size() const;
      /*! Retreive the correct cell given the step position */
      bool retrieve( TVector3 &, xAOD::RawCell*& ) const;
      /*! Get the cell map */ 
      const collection_map_t& operator*() const;
      /*! Sampling */
      CaloSampling::CaloSample sampling() const;
    
    private:

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
      /*! Calorimeter sampling id */
      CaloSampling::CaloSample m_sampling;

  };
}// namespace
#endif
