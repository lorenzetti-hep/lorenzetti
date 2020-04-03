#ifndef CaloCell_h
#define CaloCell_h

/** simulator includes **/
#include "CaloCell/enumeration.h"
#include "GaugiKernel/macros.h"

/** geant 4 includes **/
#include "G4Step.hh"
#include "globals.hh"

/** standard includes **/
#include <vector>

namespace xAOD{

  
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
                std::vector<float> time,
                std::string cell_hash);

      /** Destructor **/
      ~CaloCell();
      
      
      /** Cell eta center **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_eta, setEta, eta );
      /** Cell phi center **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_phi, setPhi, phi );
      /** Cell delta eta **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_deta, setDeltaEta , deltaEta);
      /** Cell delta phi **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_dphi, setDeltaPhi, deltaPhi );
      /** Cell r minimal in the plane xy **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_rmin, setRmin, rmin );
      /** Cell r maximal in the plane xy **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_rmax, setRmax, rmax );
      /** Get the cell hash **/
      PRIMITIVE_SETTER_AND_GETTER( std::string, m_hash, setHash, hash );
      /** Tranverse energy **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_et, setEt, et );
      /** The cell estimated energy **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_energy, setEnergy, energy );
      /** raw transverse energy **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_rawEt, setRawEt, rawEt );
      /** The raw energy **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_rawEnergy, setRawEnergy, rawEnergy );
      /** The truth energy from the main event **/ 
      PRIMITIVE_SETTER_AND_GETTER( float, m_truthRawEnergy, setTruthRawEnergy, truthRawEnergy );
      /** Raw energy samples for each bunch crossing. **/
      PRIMITIVE_SETTER_AND_GETTER( std::vector<float>, m_rawEnergySamples, setRawEnergySamples, rawEnergySamples );
      /** The integrated pulse center in bunch zero **/
      PRIMITIVE_SETTER_AND_GETTER( std::vector<float>, m_pulse, setPulse, pulse );
      /** The sampling (EM1,EM2,EM3,HAD1,HAD2 or HAD3) **/
      PRIMITIVE_SETTER_AND_GETTER( CaloSampling::CaloSample, m_sampling, setSampling, sampling );
      /** The sampling (EM1,EM2,EM3,HAD1,HAD2 or HAD3) **/
      PRIMITIVE_SETTER_AND_GETTER( std::vector<float> , m_time , setTime , time   );
      

      /** Get the calorimeter layer (LAr or Tile) **/
      CaloSampling::CaloLayer layer();
      /** Add the step point into the cell **/
      void Fill( const G4Step * );
      /** Zeroize the pulse/sample vector **/
      void clear();
      /** Allocate a new calocell and return the pointer **/
      xAOD::CaloCell* copy();

    
    private:
  
      CaloSampling::CaloSample m_sampling;
 
      float m_eta;
      float m_phi;
      float m_deta;
      float m_dphi;
      float m_rmin; // In xy plane 
      float m_rmax; // In xy plane
      /*! Estimated energy */
      float m_et;
      float m_energy;
      /*! Raw information from energy deposit */
      float m_rawEt;
      float m_rawEnergy;
      float m_truthRawEnergy;
      

      std::vector<float> m_rawEnergySamples;
      std::vector<float> m_time;
      std::vector<float> m_pulse;

      /*! Access information */
      std::string m_hash;
  };




}
#endif
