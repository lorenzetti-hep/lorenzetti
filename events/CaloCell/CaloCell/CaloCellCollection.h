#ifndef CaloCellCollection_h
#define CaloCellCollection_h

/** simulator includes **/
#include "core/enumeration.h"
#include "core/macros.h"
#include "CaloCell/CaloCell.h"


/** geant 4 includes **/
#include "G4Step.hh"
#include "globals.hh"

/** ROOT includes **/
#include "TLorentzVector.h"

/** standard includes **/
#include <vector>
#include <string>
#include <map>


namespace xAOD{

  /// Description of a calorimeter cell
  ///
  /// This is a first stab at how a calorimeter cell could behave in the
  /// xAOD EDM.
  /// @author Joao victor da Fonseca Pinto <jodafons@cern.ch, jodafons@lps.ufrj.br>
  ///
	/// $Revision: 1 $
  /// $Date: 2020-02-01$
  ///


  /// Calorimeter cell accessor used to generate the correct key
  class CaloCellAccessor
  {  
    public:

      /** Contructor **/
      CaloCellAccessor( float etmin , float etamax, float etabins, 
                        float phimin, float phimax, float phibins, 
                        float rmin  , float rmax  , int sampling);
      /** Destructor **/
      ~CaloCellAccessor(){;};
      /** Get the cell hash (key) **/
      bool getHash( float eta, float phi, float r, std::string & );

    private:
      
      std::vector<float> m_eta_bins;
      std::vector<float> m_phi_bins;
      float m_rmin;
      float m_rmax;
      int m_sampling;
  };




  class CaloCellCollection
  {  
    public:
      
      /** Contructor **/
      CaloCellCollection();
      /** Destructor **/
      ~CaloCellCollection();
      /** Add a new calo cell into the collection **/
      void push_back( xAOD::CaloCell* );
      /** Add the cell accessor **/
      void push_back( xAOD::CaloCellAccessor* );
      /** Erase all calo cells **/
      void clear();
      /** Return the number of cells into this collection **/
      size_t size();
      /** Get a vec of cells **/
      std::vector< xAOD::CaloCell* > getCollection(); 
      /** Get a vec of cells for a specific layer **/
      std::vector< xAOD::CaloCell* > getCollection( CaloSampling::CaloSample ); 
      /** Return the correct cell given a particle position **/
      bool retrieve( TLorentzVector &, xAOD::CaloCell* );

    private:

      /** The cells collection **/
      std::map< std::string , xAOD::CaloCell* > m_collection;
      /** The layer accessor **/
      std::vector< xAOD::CaloCellAccessor* > m_collectionAccessor;
  
  };





}
#endif
