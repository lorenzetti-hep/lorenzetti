#ifndef CaloDetDescriptor_h
#define CaloDetDescriptor_h

#include "CaloCell/enumeration.h"
#include "GaugiKernel/EDM.h"
#include "GaugiKernel/macros.h"
#include "G4Step.hh"
#include "globals.hh"



namespace xAOD{

  class CaloDetDescriptor: public Gaugi::EDM
  {  
    public:

      /** Contructor **/
      CaloDetDescriptor( 
               // eta/phi position at the detector, deta/dphi size into the detector, radius
               float eta, float phi, float deta, float dphi,
               // Hash
               unsigned long int hash,
               // cell identification
               CaloSampling sampling, 
               Detector detector,
               // bunch crossing information
               float bc_duration, 
               int bcid_start, 
               int bcid_end );

      /** Destructor **/
      ~CaloDetDescriptor()=default;
      


      /*
       * Cell identification
       */

      /*! Cell eta center */
      PRIMITIVE_SETTER_AND_GETTER( float, m_eta, setEta, eta );
      /*! Cell phi center */
      PRIMITIVE_SETTER_AND_GETTER( float, m_phi, setPhi, phi );
      /*! Cell delta eta */
      PRIMITIVE_SETTER_AND_GETTER( float, m_deta, setDeltaEta , deltaEta);
      /*! Cell delta phi */
      PRIMITIVE_SETTER_AND_GETTER( float, m_dphi, setDeltaPhi, deltaPhi );
      /*! Cell hash */
      PRIMITIVE_SETTER_AND_GETTER( unsigned long int, m_hash, setHash, hash );
      /*! Cell sampling id */
      PRIMITIVE_SETTER_AND_GETTER( CaloSampling  , m_sampling , setSampling   , sampling  );
      /*! Cell layer id */
      PRIMITIVE_SETTER_AND_GETTER( Detector  , m_detector  , setDetector    , detector   );
     

      /*
       * Energy values
       */


      /*! Estimated energy from OF **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_e, setE, e );
      /*! Estimated time of flight from OF **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_tau, setTau, tau );

      /*! Energy deposity from simulated hits **/
      float edep( int bc_id=0 ) const{
        if (m_edep.count(bc_id)){
          return m_edep.at(bc_id);
        }else{// return zero case bc not exist
          return 0;
        }
      }

      void edep( int bc_id, float e ){
        m_edep[bc_id] += e;
      }

      /*
       * Time of flight (Global time from G4)
       */

      /*! Time of flight from simulated hits **/
      float tof( int bc_id=0 ) const{
        if (m_tof.count(bc_id)){
          return m_tof.at(bc_id);
        }else{// return zero case bc not exist
          return 0;
        }
      }
      void tof( int bc_id, float t ){
        m_tof[bc_id] = t;
      }



      /*
       * Bunch crossing information
       */

      /*! Bunch crossing id start */
      PRIMITIVE_SETTER_AND_GETTER( int, m_bcid_start  , set_bcid_start  , bcid_start    );
      /*! Bunch crossing id end */
      PRIMITIVE_SETTER_AND_GETTER( int, m_bcid_end    , set_bcid_end    , bcid_end      );
      /* Time space (in ns) between two bunch crossings */
      PRIMITIVE_SETTER_AND_GETTER( float, m_bc_duration , set_bc_duration , bc_duration );
      
      
      /*! Integrated pulse in bunch crossing zero */
      PRIMITIVE_SETTER_AND_GETTER( std::vector<float>, m_pulse, setPulse, pulse );
      /*! Time (in ns) for each bunch crossing */
      PRIMITIVE_SETTER_AND_GETTER( std::vector<float> , m_time , setTime , time   );


      /*
       * Eletronic pulse information
       */


      /*! get pulse for each bunch crossing (centered in bunch zero)*/
      std::vector<float> pulse( int bc_id ) const {
        // TODO: We should include some protection here
        return m_pulsePerBunch.at(bc_id);
      }


      /*! set pulse for each bunch crossing */
      void setPulse( int bc_id , std::vector<float> pulse ){ 
        m_pulsePerBunch[bc_id] = pulse;
      };

      CaloDetDescriptor * copy();


    private:
 
 

      /*! id sample */
      CaloSampling m_sampling;
      /*! id layer */
      Detector m_detector;

      
      /*! eta center */
      float m_eta;
      /*! phi center */
      float m_phi;
      /*! delta eta */
      float m_deta;
      /*! delta phi */
      float m_dphi;


      /*! The estimated energy from OF in bcid=0 */
      float m_e;
      /*! The estimated time of flight from OF in bcid=0 */
      float m_tau;
      /*! Digitalized integrated pulse for the main event (bcid=0) */
      std::vector<float> m_pulse;


      /*! bunch crossing start id */
      int m_bcid_start;
      /*! bunch crossing end id */
      int m_bcid_end;
      /*! bunch crossing space in ns between two bunchs */
      float m_bc_duration;


      /*! time (in ns) for each bunch between bcid_start and bcid_end */
      std::vector<float> m_time;
      /*! energy deposit between bcid_start and bcid_end */
      std::map< int, float> m_edep;
      /*! time of flight of a hit in the cell between bcid_start and bcid_end */
      std::map< int, float> m_tof;
      /*! Digitalized pulse for each bunch between bcid_start and bcid_end */
      std::map< int, std::vector<float> > m_pulsePerBunch;


      /*! Access information unique ID number */
      unsigned long int m_hash;

  };

}
#endif
