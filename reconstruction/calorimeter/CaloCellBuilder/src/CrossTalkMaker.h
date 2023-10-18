#ifndef CrossTalkMaker_h
#define CrossTalkMaker_h

#include "GaugiKernel/AlgTool.h"
#include "GaugiKernel/Algorithm.h" 
#include "GaugiKernel/DataHandle.h"
#include "GaugiKernel/EDM.h"
#include "CaloCell/CaloDetDescriptor.h"
#include "CaloCell/CaloDetDescriptorCollection.h"
#include "TF1.h"



class CrossTalkMaker : public Gaugi::Algorithm
{
  public:
    /** Constructor **/
    CrossTalkMaker( std::string name );
    // virtual ~CrossTalkMaker();
    ~CrossTalkMaker()=default;

    virtual StatusCode initialize() override;    
    /*! Book all histograms into the current storegate **/
    virtual StatusCode bookHistograms( SG::EventContext &ctx ) const override;
    /*! execute before start the step action **/
    virtual StatusCode pre_execute( SG::EventContext &ctx ) const override;
    /*! Execute in step action step from geant core **/
    virtual StatusCode execute( SG::EventContext &ctx , const G4Step *step) const override;
    /*! Execute in ComponentAccumulator **/
    virtual StatusCode execute( SG::EventContext &ctx , int /*evt*/ ) const override;
    /*! execute after the step action **/ 
    virtual StatusCode post_execute( SG::EventContext &ctx ) const override;

    virtual StatusCode finalize() override;
    /*! Fill all histograms into the current store gate */
    virtual StatusCode fillHistograms( SG::EventContext & /*ctx*/ ) const override;
    /*! Add tools to be executed into the post execute step. The order is matter here */
    void push_back( Gaugi::AlgTool *);



  private:

    double  XTalk(double x, bool type) const;
    double  CellFunction(double x, bool type) const;
    float   XTalkTF( float sample, int samp_index, bool diagonal, bool inductive) const;

    /*! The tool list that will be executed into the post execute step */
    std::vector< Gaugi::AlgTool* > m_toolHandles;

    float m_minEnergy;
    std::string m_collectionKey;
    std::string m_xtcollectionKey;
    std::string m_histPath;

    uint m_Nsamples = 5 ;
    uint m_tSamp = 25 ;

    // cts modelling xtalk
    double m_Cx      =  47.000 ;
    double m_Rf      =   0.078 ;
    double m_Rin     =   1.200 ;
    double m_taud    =  15.820 ;
    double m_taupa   =  17.310 ;
    double m_td      = 420.000 ;
    double m_tmax2   = 600.000 ;
    double m_C1      =  50.000 ;

    // This value normalize amplitude of the Xt_C to unit
    double m_ToNormXtC = 0.022206 ;       
    // This value normalize amplitude of the Xt_L to unit
    //double m_ToNormXtL = 0.0539463;
    // Noise amplitude on LAr is 50 MeV

    // XTalk amplitude on % values of the Energy
    double m_AmpXt_C; //4.2/100  ;
    double m_AmpXt_L; //2.3/100  ;       // XTalk amplitude on % values of the Energy
    double m_AmpXt_R; //1.0/100  ;       // XTalk amplitude on % values of the Energy
    double m_AmpNoise  = 50 ;             // Noise amplitude 50 MeV
    double tau_0_mean  = 0 ;
    double tau_std     = 0.5 ;

};

#endif