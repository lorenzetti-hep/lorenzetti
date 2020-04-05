#ifndef CaloCellMaker_h
#define CaloCellMaker_h

#include "GaugiKernel/AlgTool.h"
#include "GaugiKernel/Algorithm.h"
#include "GaugiKernel/DataHandle.h"
#include "CaloCell/CaloCell.h"
#include "CaloCellCollection.h"


class CaloCellMaker : public Gaugi::Algorithm
{
  public:
  
    /** Contructor **/
    CaloCellMaker( std::string );
    
    /** Destructor **/
    ~CaloCellMaker();
    
    /*! initialize the algorithm **/
    virtual StatusCode initialize() override;
    
    /*! Execute in step action step from geant core **/
    virtual StatusCode execute( EventContext *ctx , const G4Step *step) const override;
    
    /*! execute before start the step action **/
    virtual StatusCode pre_execute( EventContext *ctx ) const override;
    
    /*! execute after the step action **/ 
    virtual StatusCode post_execute( EventContext *ctx ) const override;
    
    /*! fill hisogram in the end **/
    virtual StatusCode fillHistograms( EventContext *ctx ) const override;
    
    /*! finalize the algorithm **/ 
    virtual StatusCode finalize() override;


    /*! Add tools to be executed into the post execute step. The order is matter here */
    void push_back( Gaugi::AlgTool *tool ){ m_toolHandles.push_back(tool); };


  private:
   
    /*
     * Properties
     */

    std::string m_outputKey;
    
    /*! The tool list that will be executed into the post execute step */
    std::vector< Gaugi::AlgTool* > m_toolHandles;
    

    /*
     * Internal usage
     */

    /*! Sampling id for this reconstruction */
    int m_sampling;
    /*! The start bunch crossing id for energy estimation */
    int m_bcid_start;
    /*! The end bunch crossing id for energy estimation */
    int m_bcid_end;
    /*! The truth bunch crossing id */
    int m_bcid_truth;
    /*! The number of samples per bunch crossing */
    int m_bc_nsamples;
    /*! The time space (in ns) between two bunch crossings */
    float m_bc_duration;

  
    float m_eta_min;
    float m_eta_max, 
    float m_eta_bins, 
    float m_phi_min, 
    float m_phi_max, 
    float m_phi_bins, 
    float m_rmin, 
    float m_rmax;
  
};



#endif
