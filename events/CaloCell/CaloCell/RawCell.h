#ifndef RawCell_h
#define RawCell_h

#include "CaloCell/enumeration.h"
#include "GaugiKernel/EDM.h"
#include "GaugiKernel/macros.h"
#include "G4Step.hh"
#include "globals.hh"


namespace xAOD{

  class RawCell: public Gaugi::EDM
  {  
    public:

      /** Contructor **/
      RawCell( float eta, float phi, float deta, float dphi, float radius_min, float radius_max,
               std::string hash, int channel_eta, int channel_phi, CaloSampling::CaloSample sampling, 
               float bc_duration, int bc_nsamples, int bcid_start, int bcid_end, int bcid_truth );

      /** Destructor **/
      ~RawCell()=default;
      /*! Fill the deposit energy into the cell */
      void Fill( const G4Step * );
      /** Zeroize the pulse/sample vectors **/
      void clear();

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
      /*! Estimated energy **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_energy, setEnergy, energy );
      /*! Truth raw energy calculated on top of the special bunch crossing */ 
      PRIMITIVE_SETTER_AND_GETTER( float, m_truthRawEnergy, setTruthRawEnergy, truthRawEnergy );
      /*! Bunch crossing id start */
      PRIMITIVE_SETTER_AND_GETTER( int, m_bcid_start  , set_bcid_start  , bcid_start    );
      /*! Bunch crossing id end */
      PRIMITIVE_SETTER_AND_GETTER( int, m_bcid_end    , set_bcid_end    , bcid_end      );
      /*! Number of samples per bunch crossing */
      PRIMITIVE_SETTER_AND_GETTER( int, m_bc_nsamples , set_bc_nsamples , bc_nsamples   );
      /*! Bunch crossing id truth */
      PRIMITIVE_SETTER_AND_GETTER( int, m_bcid_truth , set_bcid_truth , bcid_truth );
      /* Time space (in ns) between two bunch crossings */
      PRIMITIVE_SETTER_AND_GETTER( float, m_bc_duration , set_bc_duration , bc_duration );
      /*! Raw energy samples for each bunch crossing. */
      PRIMITIVE_SETTER_AND_GETTER( std::vector<float>, m_rawEnergySamples, setRawEnergySamples, rawEnergySamples );
      /*! Integrated pulse in bunch crossing zero */
      PRIMITIVE_SETTER_AND_GETTER( std::vector<float>, m_pulse, setPulse, pulse );

      /*! Time (in ns) for each bunch crossing */
      PRIMITIVE_SETTER_AND_GETTER( std::vector<float> , m_time , setTime , time   );


      /*! get pulse for each bunch crossing */
      std::vector<float> pulse( int bc_id ) const {return m_pulsePerBunch.at(bc_id);};
      /*! set pulse for each bunch crossing */
      void setPulsePerBunch( int bc_id , std::vector<float> pulse ){ m_pulsePerBunch[bc_id] = pulse;};


      PRIMITIVE_SETTER_AND_GETTER( int, m_channel_eta , setChannelEta , channelEta   );
      PRIMITIVE_SETTER_AND_GETTER( int, m_channel_phi , setChannelPhi , channelPhi   );


    private:
 
      /*! id sampling */
      CaloSampling::CaloSample m_sampling;
      /*! the eta id of this cell*/
      int m_channel_eta;
      /*! the phi id of this cell*/
      int m_channel_phi;


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
      /* Energy */
      float m_energy;
      /*! The total Energy deposit betweeen bcid_start and bcid_end */
      float m_rawEnergy;
      /*! The total energy deposit in bcid_truth */
      float m_truthRawEnergy;
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
      /*! energy deposit for each sample calculated from geant between bcid_start and bcid_end */
      std::vector<float> m_rawEnergySamples;
      /*! time (in ns) for each bunch between bcid_start and bcid_end */
      std::vector<float> m_time;
      /*! Digitalized integrated pulse for the main event (bcid zero) */
      std::vector<float> m_pulse;
      /*! Digitalized pulse per bunch */
      std::map< int, std::vector<float> > m_pulsePerBunch;
      /*! Access information */
      std::string m_hash;
  };

}
#endif
