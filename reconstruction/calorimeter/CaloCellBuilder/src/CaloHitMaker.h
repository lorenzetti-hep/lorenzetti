#ifndef CaloHitMaker_h
#define CaloHitMaker_h

#include "GaugiKernel/Algorithm.h"
#include "GaugiKernel/DataHandle.h"
#include "GaugiKernel/AlgTool.h"
#include "CaloHit/CaloHit.h"



class CaloHitMaker : public Gaugi::Algorithm
{

  typedef std::map<unsigned long int, xAOD::CaloHit* > collection_map_t;

  public:
  
    /** Contructor **/
    CaloHitMaker( std::string name );
    /** Destructor **/
    ~CaloHitMaker()=default;
    
    /*! initialize the algorithm **/
    virtual StatusCode initialize() override;
    /*! Book all histograms into the current storegate **/
    virtual StatusCode bookHistograms( SG::EventContext &ctx ) const override;
    /*! Execute in step action step from geant core **/
    virtual StatusCode execute( SG::EventContext &ctx , const G4Step *step) const override;
    /*! Execute in ComponentAccumulator **/
    virtual StatusCode execute( SG::EventContext &ctx , int /*evt*/ ) const override;
    /*! execute before start the step action **/
    virtual StatusCode pre_execute( SG::EventContext &ctx ) const override;
    /*! execute after the step action **/ 
    virtual StatusCode post_execute( SG::EventContext &ctx ) const override;
    /*! fill hisogram in the end **/
    virtual StatusCode fillHistograms( SG::EventContext &ctx ) const override;
    /*! finalize the algorithm **/ 
    virtual StatusCode finalize() override;

  private:
   
    int find( const std::vector<float> &vec, float value) const;
    unsigned long int hash(unsigned bin) const;

    /*! collection key */
    std::string m_collectionKey; // output

    std::vector<float> m_etaBins; 
    std::vector<float> m_phiBins; 
    float m_rMin;
    float m_rMax; 
    float m_zMin;
    float m_zMax;
    float m_noiseStd;

    /*! Sampling id for this reconstruction */
    int m_sampling;
    /*! Sampling id segment */
    int m_segment;
    /*! Detector id for this sampling*/
    int m_detector;
    /*! The start bunch crossing id for energy estimation */
    int m_bcid_start;
    /*! The end bunch crossing id for energy estimation */
    int m_bcid_end;
    /*! The time space (in ns) between two bunch crossings */
    float m_bc_duration;
    /*! Base histogram path */
    std::string m_histPath;
    /*! detailed histogram flags */
    bool m_detailedHistograms;

    unsigned int m_nEtaBins;
    unsigned int m_nPhiBins;
};


#endif
