#ifndef CaloCellCollection_h
#define CaloCellCollection_h


#include "GaugiKernel/DataHandle.h"
#include "CaloCell/CaloCell.h"
#include "TVector3.h"
#include <string>
#include <map>


namespace xAOD{

  class CaloCellCollection : public DataHandle
  {  
    typedef std::map<std::string, std::unique_ptr<xAOD::CaloCell> > collection_map_t;

    public:

      /*! Contructor */
      CaloCellCollection( float etamin, float etamax , float etabins, 
                          float phimin, float phimax , float phibins, 
                          float rmin  , float rmax   , int   sampling);
 
      /*! Destructor */
      ~CaloCellCollection();

      /*! Add a new calo cell into the collection */
      void push_back( xAOD::CaloCell* );
      /*! Zeroize all calo cells */
      void clear();
      /*! Return the number of cells into this collection */
      size_t size();
      /*! Retreive the correct cell given the step position */
      bool retrieve( TVector3 &, xAOD::CaloCell*& );
      /*! Get the cell map */ 
      collection_map_t& operator*();

     
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
      float m_radius_rmax;
      /*! Calorimeter sampling id */
      int m_sampling;

  };





}// namespace
#endif
