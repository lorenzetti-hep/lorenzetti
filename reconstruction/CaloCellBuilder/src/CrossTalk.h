#ifndef CrossTalk_h
#define CrossTalk_h

#include "GaugiKernel/AlgTool.h"
#include "GaugiKernel/EDM.h"
#include "TF1.h"



class CrossTalk : public Gaugi::AlgTool
{
  public:
    /** Constructor **/
    CrossTalk( std::string name );
    virtual ~CrossTalk();
    virtual StatusCode initialize() override;
    virtual StatusCode finalize() override;

    virtual StatusCode execute( SG::EventContext &ctx, Gaugi::EDM * ) const override;
    virtual StatusCode fillHistograms( SG::EventContext &ctx, std::vector<float> samples_ind, std::vector<float> samples_cap, std::vector<float> samples_signal, std::vector<float> samples_signal_xtalk ) const;
    double  XTalk(double x, bool type) const ;
    double  CellFunction(double x, bool type) const;
    // void    XTalkTF(float &ind_part, float &cap_part, float sample, int samp_index, bool diagonal);
    float   XTalkTF( float sample, int samp_index, bool diagonal, bool inductive) const;

    // void    BuildSampCluster(vector<double> BaseAmpXTc, vector<double> BaseAmpXTl,  vector<double> BaseAmpXTr, vector<double> delayPerCell, vector<double> &XTcSamples, vector<double> &XTlSamples, vector<double> &XTrSamples, vector<double> &CellSigSamples, vector<double> &SampClusNoise, uint m_Nsamples, double m_tau_0 ) const;

  private:

    float m_minEnergy;
    std::string m_collectionKey;
    std::string m_histPath;

    uint m_Nsamples = 5 ;
    uint m_tSamp = 25 ;

    // booking variables
    mutable std::vector < float > samples_xtalk_ind    ;
    mutable std::vector < float > samples_xtalk_cap    ;
    mutable std::vector < float > samples_signal       ;
    mutable std::vector < float > samples_signal_xtalk ;

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
    double m_AmpXt_C   = 4.2/100  ;
    double m_AmpXt_L   = 2.3/100  ;       // XTalk amplitude on % values of the Energy
    double m_AmpXt_R   = 1.0/100  ;       // XTalk amplitude on % values of the Energy
    double m_AmpNoise  = 50 ;             // Noise amplitude 50 MeV
    double tau_0_mean  = 0 ;
    double tau_std     = 0.5 ;

};

#endif

