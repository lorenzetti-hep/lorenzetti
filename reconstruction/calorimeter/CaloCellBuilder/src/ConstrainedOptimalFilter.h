#ifndef ConstrainedOptimalFilter_h
#define ConstrainedOptimalFilter_h

#include "GaugiKernel/AlgTool.h"
#include "GaugiKernel/EDM.h"
#include "TMatrixD.h"
#include "TVectorD.h"



class ConstrainedOptimalFilter : public Gaugi::AlgTool
{

  public:
    /** Constructor **/
    ConstrainedOptimalFilter( std::string name );
    virtual ~ConstrainedOptimalFilter();
    virtual StatusCode initialize() override;
    virtual StatusCode finalize() override;
    void ReadShaper( std::string filepath );
    void GeneratePulse(  std::vector<float> &pulse) const;

    virtual StatusCode execute( SG::EventContext &ctx, Gaugi::EDM * ) const override;

  private:

    /*! optimal filter weights */
    int m_startSamplingBC;
    std::string m_pulsepath;
    std::vector<float> m_shaper;
    float m_shaperResolution;
    std::vector<float> m_timeSeries;
    float m_threshold;
    int m_shaperZeroIndex;
    int m_nsamples;
    float m_samplingRate;
};

#endif




