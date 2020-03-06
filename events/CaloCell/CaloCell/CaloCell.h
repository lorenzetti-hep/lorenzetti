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
  /// This is a first stab at how a calorimeter cell could behave in the
  /// xAOD EDM.
  /// @author Joao victor da Fonseca Pinto <jodafons@cern.ch, jodafons@lps.ufrj.br>
	/// $Revision: 1 $
  /// $Date: 2020-02-01$
  
  class CaloCell
  {  
    public:

      /** Constructor **/
      CaloCell();

      /** Contructor **/
      CaloCell( float eta_center,  float phi_center, 
                float delta_eta, float delta_phi, 
                float rmin, float rmax,
                CaloSampling::CaloSample sampling,
                std::vector<float> timestamp,
                std::string cell_hash);

      /** Destructor **/
      ~CaloCell();

      /** Add the step point into the cell **/
      void Fill( const G4Step * );
      /** Zeroize the pulse/sample vector **/
      void clear();
      /** Allocate a new calocell and return the pointer **/
      xAOD::CaloCell* copy();

      /** Cell eta center **/
      PRIMITIVE_SETTER_AND_GETTER( float      , m_eta   , setEta      , eta       );
      /** Cell phi center **/
      PRIMITIVE_SETTER_AND_GETTER( float      , m_phi   , setPhi      , phi       );
      /** Cell delta eta **/
      PRIMITIVE_SETTER_AND_GETTER( float      , m_deta  , setDeltaEta , deltaEta  );
      /** Cell delta phi **/
      PRIMITIVE_SETTER_AND_GETTER( float      , m_dphi  , setDeltaPhi , deltaPhi  );
      /** Cell r minimal in the plane xy **/
      PRIMITIVE_SETTER_AND_GETTER( float      , m_rmin  , setRmin     , rmin      );
      /** Cell r maximal in the plane xy **/
      PRIMITIVE_SETTER_AND_GETTER( float      , m_rmax  , setRmax     , rmax      );
      /** Get the cell hash **/
      PRIMITIVE_SETTER_AND_GETTER( std::string, m_hash  , setHash     , hash      );
      /** Tranverse energy **/
      PRIMITIVE_SETTER_AND_GETTER( float      , m_et    , setEt       , et        );
      /** The cell estimated energy **/
      PRIMITIVE_SETTER_AND_GETTER( float      , m_energy, setEnergy   , energy    );
      /** raw transverse energy **/
      PRIMITIVE_SETTER_AND_GETTER( float      , m_rawEt , setRawEt    , rawEt     );
      /** The raw energy **/
      PRIMITIVE_SETTER_AND_GETTER( float      , m_rawEnergy, setRawEnergy, rawEnergy );


      /** Raw energy samples for each bunch crossing. **/
      PRIMITIVE_SETTER_AND_GETTER( std::vector<float>   , m_rawEnergySamples  , setRawEnergySamples  , rawEnergySamples );
      /** The integrated pulse center in bunch zero **/
      PRIMITIVE_SETTER_AND_GETTER( std::vector<float>   , m_pulse             , setPulse, pulse );

      /** The sampling (EM1,EM2,EM3,HAD1,HAD2 or HAD3) **/
      PRIMITIVE_SETTER_AND_GETTER( CaloSampling::CaloSample , m_sampling , setSampling , sampling   );
      
      /** Get the calorimeter layer (LAr or Tile) **/
      CaloSampling::CaloLayer layer();
      
      /** The sampling (EM1,EM2,EM3,HAD1,HAD2 or HAD3) **/
      PRIMITIVE_SETTER_AND_GETTER( std::vector<float> , m_timestamp , setTimestamp , timestamp   );
      
    private:
  
      /* The calorimeter layer */
      CaloSampling::CaloSample m_sampling;
 
      /* eta center */
      float m_eta;
      /* phi center */
      float m_phi;
      /* delta eta cell */
      float m_deta;
      /* delta phi cell */
      float m_dphi;
      float m_rmin; // In xy plane 
      float m_rmax; // In xy plane
     
      /* Estimated transverse energy **/
      float m_et;
      /* Energy cell after shaper */
      float m_energy;

      /* raw transverse energy **/
      float m_rawEt;
      /* raw energy (without shaper) */
      float m_rawEnergy;
      
      /* Energy for each bunch crossing */
      std::vector<float> m_rawEnergySamples;
      /* time bins */
      std::vector<float> m_timestamp;
      
      /* Integrated pulse */
      std::vector<float> m_pulse;

      /* Cell hash */
      std::string m_hash;
  };




}
#endif
