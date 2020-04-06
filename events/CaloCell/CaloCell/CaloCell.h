#ifndef CaloCell_h
#define CaloCell_h

/** simulator includes **/
#include "CaloCell/enumeration.h"
#include "GaugiKernel/macros.h"

/** geant 4 includes **/
#include "G4Step.hh"
#include "globals.hh"
#include <vector>

namespace xAOD{

  
  class CaloCell
  {  
    public:

      /** Constructor **/
      CaloCell();

      /** Contructor **/
      CaloCell( float eta,  
                float phi, 
                float deta, 
                float dphi, 
                float radius_min, 
                float radius_max,
                std::string hash,
                CaloSampling::CaloSample sampling,
                float bc_duration,
                int bc_nsamples,
                int bcid_start,
                int bcid_end,
                int bcid_truth );

      /** Destructor **/
      ~CaloCell();
 
      /*! Fill the deposit energy into the cell */
      void Fill( const G4Step * );

      /** Zeroize the pulse/sample vectors **/
      void clear();
      
      /*! Allocate a new calocell and return the pointer */
      xAOD::CaloCell* copy();
     
      
      /*
       * Cell position and information
       */

      /*! Cell eta center */
      PRIMITIVE_SETTER_AND_GETTER( float, m_eta, setEta, eta );
      /*! Cell phi center */
      PRIMITIVE_SETTER_AND_GETTER( float, m_phi, setPhi, phi );
      /*! Cell delta eta */
      PRIMITIVE_SETTER_AND_GETTER( float, m_deta, setDeltaEta , deltaEta);
      /*! Cell delta phi */
      PRIMITIVE_SETTER_AND_GETTER( float, m_dphi, setDeltaPhi, deltaPhi );
      /*! Cell minimal radius in the plane xy */
      PRIMITIVE_SETTER_AND_GETTER( float, m_radius_min, setRmin, rmin );
      /*! Cell maximal radius in the plane xy */
      PRIMITIVE_SETTER_AND_GETTER( float, m_radius_max, setRmax, rmax );
      /*! Cell hash */
      PRIMITIVE_SETTER_AND_GETTER( std::string, m_hash, setHash, hash );
      /*! Cell sampling id */
      PRIMITIVE_SETTER_AND_GETTER( CaloSampling::CaloSample, m_sampling, setSampling, sampling );
 

      /*
       * Estimated energy
       */

      /*! Estimated energy **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_energy, setEnergy, energy );
      PRIMITIVE_SETTER_AND_GETTER( float, m_et, setEt, et );
      
      
      /*
       * Raw energy
       */

      /*! Raw energy (without estimation) */
      PRIMITIVE_SETTER_AND_GETTER( float, m_rawEnergy, setRawEnergy, rawEnergy );
      PRIMITIVE_SETTER_AND_GETTER( float, m_rawEt, setRawEt, rawEt );
     


      /*
       * Truth energy extracted from the main event without any contamination 
       */

      /*! Truth raw energy calculated on top of the special bunch crossing */ 
      PRIMITIVE_SETTER_AND_GETTER( float, m_truthRawEnergy, setTruthRawEnergy, truthRawEnergy );

      
      
      
      /*
       * Bunch crossing information
       */

      PRIMITIVE_SETTER_AND_GETTER( int, m_bcid_start  , set_bcid_start  , bcid_start    );
      PRIMITIVE_SETTER_AND_GETTER( int, m_bcid_end    , set_bcid_end    , bcid_end      );
      PRIMITIVE_SETTER_AND_GETTER( int, m_bc_nsamples , set_bc_nsamples , bc_nsamples   );
      PRIMITIVE_SETTER_AND_GETTER( int, m_bcid_truth , set_bcid_truth , bcid_truth );
      PRIMITIVE_SETTER_AND_GETTER( float, m_bc_duration , set_bc_duration , bc_duration );


      /*
       * Pulse information
       */

      /*! Raw energy samples for each bunch crossing. */
      PRIMITIVE_SETTER_AND_GETTER( std::vector<float>, m_rawEnergySamples, setRawEnergySamples, rawEnergySamples );
      /*! Integrated pulse in bunch crossing zero */
      PRIMITIVE_SETTER_AND_GETTER( std::vector<float>, m_pulse, setPulse, pulse );
      /*! Time (in ns) for each bunch crossing */
      PRIMITIVE_SETTER_AND_GETTER( std::vector<float> , m_time , setTime , time   );
 
    


    private:
 
      /*
       * Cell information
       */
 
      /*! id sampling */
      CaloSampling::CaloSample m_sampling;
      /*! eta center */
      float m_eta;
      /*! phi center */
      float m_phi;
      /*! delta eta */
      float m_deta;
      /*! delta phi */
      float m_dphi;
      /*! In plane xy */
      float m_radius_min; 
      /*! In plane xy */
      float m_radius_max;


      float m_energy;
      float m_rawEnergy;
      float m_truthRawEnergy;

      
      float m_rawEt;
      float m_et;


      /*
       * Bunch crossing information 
       */

      /*! bunch crossing start id */
      int m_bcid_start;
      /*! bunch crossing end id */
      int m_bcid_end;
      /*! number of samples per bunch crossing */
      int m_bc_nsamples;
      /*! truth bunch crossing */
      int m_bcid_truth;
      /*! bunch crossing space in ns between two bunchs */
      float m_bc_duration;

      /*
       * Pulse information
       */

      /*! energy deposit for each sample calculated from geant */
      std::vector<float> m_rawEnergySamples;
      /*! time (in ns) for each bunch */
      std::vector<float> m_time;
      /*! Digitalized pulse for the main event */
      std::vector<float> m_pulse;



      /*! Access information */
      std::string m_hash;
  };




}
#endif
