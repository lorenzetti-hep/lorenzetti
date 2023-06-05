#ifndef SpacePoint_h
#define SpacePoint_h

#include "SpacePoint/enumeration.h"
#include "GaugiKernel/EDM.h"
#include "GaugiKernel/macros.h"
#include "G4Step.hh"
#include "globals.hh"



namespace xAOD{

  class SpacePoint: public Gaugi::EDM
  {  
    public:

      SpacePoint():EDM(){};

      /** Contructor **/
      SpacePoint( 
                  // eta/phi position at the detector, deta/dphi size into the detector, radius
                  float x, 
                  float y, 
                  float z, 
                  float phi,
                  float dz,
                  float dphi,
                  // Hash
                  unsigned long int hash,
                  // Hit identification
                  InnerSampling sampling, 
                  InnerDetector detector,
                  // bunch crossing information
                  float bc_duration, 
                  int bcid_start, 
                  int bcid_end 
                 );

      /** Destructor **/
      ~SpacePoint()=default;
      
      /*! Fill the deposit energy into the Hit */
      void fill( const G4Step * );
      /** Zeroize the pulse/sample vectors **/
      void clear();


      /*
       * Hit identification
       */

      /*! Hit eta center */
      PRIMITIVE_SETTER_AND_GETTER( float, m_x, setX, x );
      PRIMITIVE_SETTER_AND_GETTER( float, m_y, setY, y );
      PRIMITIVE_SETTER_AND_GETTER( float, m_z, setZ, z );

      /*! Hit phi center */
      PRIMITIVE_SETTER_AND_GETTER( float, m_phi, setPhi, phi );
      /*! Hit delta eta */
      PRIMITIVE_SETTER_AND_GETTER( float, m_dz, setDeltaZ , deltaZ);
      /*! Hit delta phi */
      PRIMITIVE_SETTER_AND_GETTER( float, m_dphi, setDeltaPhi, deltaPhi );


      /*! Hit hash */
      PRIMITIVE_SETTER_AND_GETTER( unsigned long int, m_hash, setHash, hash );
      /*! Hit sampling id */
      PRIMITIVE_SETTER_AND_GETTER( InnerSampling  , m_sampling , setSampling   , sampling  );
      /*! Hit layer id */
      PRIMITIVE_SETTER_AND_GETTER( InnerDetector  , m_detector  , setDetector    , detector   );
     



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
       * Bunch crossing information
       */

      /*! Bunch crossing id start */
      PRIMITIVE_SETTER_AND_GETTER( int, m_bcid_start  , set_bcid_start  , bcid_start    );
      /*! Bunch crossing id end */
      PRIMITIVE_SETTER_AND_GETTER( int, m_bcid_end    , set_bcid_end    , bcid_end      );
      /* Time space (in ns) between two bunch crossings */
      PRIMITIVE_SETTER_AND_GETTER( float, m_bc_duration , set_bc_duration , bc_duration );
      /*! Time (in ns) for each bunch crossing */
      PRIMITIVE_SETTER_AND_GETTER( std::vector<float> , m_time , setTime , time   );



    private:
 
      int find( float value) const ;
       

      /*! id sample */
      InnerSampling m_sampling;
      /*! id layer */
      InnerDetector m_detector;
      /*! space point position */
      float m_x, m_y, m_z, m_phi;
      /*! delta eta */
      float m_dz;
      /*! delta phi */
      float m_dphi;
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
     
      /*! Access information unique ID number */
      unsigned long int m_hash;

  };

}
#endif
