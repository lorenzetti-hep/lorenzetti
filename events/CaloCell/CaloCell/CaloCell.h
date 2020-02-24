#ifndef CaloCell_h
#define CaloCell_h

/** simulator includes **/
#include "core/enumeration.h"
#include "core/macros.h"

/** geant 4 includes **/
#include "G4Step.hh"
#include "globals.hh"

/** standard includes **/
#include <vector>

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

  class CaloCell
  {  
    public:

      CaloCell( float eta_center,  float phi_center, 
                float delta_eta, float delta_phi, 
                float rmin, float rmax,
                CaloSampling::CaloSample sampling,
                std::vector<float> tbins,
                std::string cell_hash);

      ~CaloCell();

      /** Add the step point into the cell **/
      void push_back( const G4Step * );
       
      /** Zeroize the pulse/sample vector **/
      void clear();
      
      /** Allocate a new calocell and return the pointer **/
      xAOD::CaloCell* clone();

      /** Cell eta center **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_eta_center  , setEta      , eta       );
      /** Cell phi center **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_phi_center  , setPhi      , phi       );
      /** Cell delta eta **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_delta_eta   , setDeltaEta , deltaEta  );
      /** Cell delta phi **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_delta_phi   , setDeltaPhi , deltaPhi  );
      /** Cell r minimal in the plane xy **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_rmin        , setRmin     , rmin      );
      /** Cell r maximal in the plane xy **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_rmax        , setRmax     , rmax      );
      /** Set the raw energy (without calibration) **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_rawEnergy   , setRawEnergy, rawEnergy );
      /** Raw energy samples store in this calorimeter cell **/
      PRIMITIVE_SETTER_AND_GETTER( std::vector<float>   , m_rawEnergySamples   , setRawEnergySamples  , rawEnergySamples );
      /** The sampling (EM1,EM2,EM3,HAD1,HAD2 or HAD3) **/
      PRIMITIVE_SETTER_AND_GETTER( CaloSampling::CaloSample , m_sampling , setSampling , sampling   );
      /** Get the cell hash **/
      PRIMITIVE_SETTER_AND_GETTER( std::string , m_hash , setHash , hash   );


    private:
  
      /* The calorimeter layer */
      CaloSampling::CaloSample m_sampling;
  
      float m_eta_center;
      float m_phi_center;
      float m_delta_eta;
      float m_delta_phi;
      float m_rmin; // In xy plane 
      float m_rmax; // In xy plane
      
      /* raw energy (without calibration) */
      float m_rawEnergy;
      
      /* Energy for each sample throut all bunchs */
      std::vector<float> m_rawEnergySamples;
      /* time bins */
      std::vector<float> m_tbins;
      
      /* Cell hash */
      std::string m_hash;
  };




}
#endif
