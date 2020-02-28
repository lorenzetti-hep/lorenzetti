#ifndef CaloCluster_h
#define CaloCluster_h

/** simulator includes **/
#include "CaloCell/CaloCell.h"
#include "core/macros.h"

/** geant 4 includes **/
#include "globals.hh"

/** standard includes **/
#include <vector>

// Event Object Data
namespace xAOD{
  
  /// Description of a calorimeter cluster
  ///
  /// This is a first stab at how a calorimeter cell could behave in the
  /// xAOD EDM.
  /// @author Joao victor da Fonseca Pinto <jodafons@cern.ch, jodafons@lps.ufrj.br>
  ///
	/// $Revision: 1 $
  /// $Date: 2020-02-01$
  ///
  class CaloCluster
  {  
    public:
      /** Contructor **/
      CaloCluster( float energy_center, float eta_center, float phi_center, float delta_eta, float delta_phi );
      /** Destructor **/
      ~CaloCluster();
      /** Add the calorimeter cell into the RoI collection **/
      void push_back( const xAOD::CaloCell * );
      /** Get the cell collection into this RoI **/
      const std::vector< const xAOD::CaloCell*> & getCollection();
      /** Return the number of cells inside of this cluster **/
      size_t size();
      /** release all memory holded by the cell collection into this RoI **/
      void clear();
      /** The eta center of the cluster given by the simulation (RoI) **/ 
      PRIMITIVE_SETTER_AND_GETTER( float, m_eta_center  , setEta        , eta         );
      /** The phi center of the cluster given by the simulation (RoI) **/ 
      PRIMITIVE_SETTER_AND_GETTER( float, m_phi_center  , setPhi        , phi         );
      /** The RoI eta window **/ 
			PRIMITIVE_SETTER_AND_GETTER( float, m_delta_eta   , setDeltaEta   , deltaEta    );
      /** The RoI phi window **/ 
			PRIMITIVE_SETTER_AND_GETTER( float, m_delta_phi   , setDeltaPhi   , deltaPhi    );
			/** The total energy of the RoI **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_totalEnergy , setTotalEnergy, totalEnergy );
      /** The energy center **/ 
      PRIMITIVE_SETTER_AND_GETTER( float, m_energy_center , setEnergyCenter, energyCenter );

    private:

      float m_energy_center;
      float m_eta_center;
      float m_phi_center;
      float m_delta_eta;
      float m_delta_phi;
      /* Total estimated energy into the roi in the second eletromagnetic layer*/
      float m_totalEnergy;
      /* All calo cells into the roi window */
      std::vector<const xAOD::CaloCell*> m_collection;
  };
}
#endif
