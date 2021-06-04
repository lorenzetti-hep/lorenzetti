#ifndef CaloHitMaker_h
#define CaloHitMaker_h

#include "GaugiKernel/Algorithm.h"
#include "GaugiKernel/DataHandle.h"
#include "GaugiKernel/AlgTool.h"


class CaloHitMaker : public Gaugi::Algorithm
{
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
    /*! execute before start the step action **/
    virtual StatusCode pre_execute( SG::EventContext &ctx ) const override;
    /*! execute after the step action **/ 
    virtual StatusCode post_execute( SG::EventContext &ctx ) const override;
    /*! fill hisogram in the end **/
    virtual StatusCode fillHistograms( SG::EventContext &ctx ) const override;
    /*! finalize the algorithm **/ 
    virtual StatusCode finalize() override;
    

  private:
   
    /*! collection key */
    std::string m_collectionKey;
    /*! event key */
    std::string m_eventKey;
    /*! Base histogram path */
    std::string m_histPath;
    /*! The path to the hit configuration file */
    std::string m_caloHitFile;
    /*! Segment index for this sample calorimeter */
    int m_segmentation;
    /*! Sampling id for this reconstruction */
    int m_sampling;
    /*! Detector id for this sampling*/
    int m_detector;
    /*! The start bunch crossing id for energy estimation */
    int m_bcid_start;
    /*! The end bunch crossing id for energy estimation */
    int m_bcid_end;
    /*! The time space (in ns) between two bunch crossings */
    float m_bc_duration;


    float m_eta_min;
    float m_eta_max; 
    float m_rmin;
    float m_rmax;  
    
    std::vector<float> m_eta_bins; 
    std::vector<float> m_phi_bins; 

    bool m_detailedHistograms;
};



#endif
