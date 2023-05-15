#ifndef PulseGenerator_h
#define PulseGenerator_h

#include "GaugiKernel/StatusCode.h"
#include "GaugiKernel/AlgTool.h"
#include "GaugiKernel/EDM.h"
#include "TRandom3.h"


class PulseGenerator : public Gaugi::AlgTool
{

  public:
    /** Constructor **/
    PulseGenerator( std::string name );
    virtual ~PulseGenerator();
    
    virtual StatusCode initialize() override;
    virtual StatusCode finalize() override;

    virtual StatusCode execute( SG::EventContext &ctx, Gaugi::EDM * ) const override;



  private:

    void ReadShaper( std::string );
    void GenerateDeterministicPulse(  std::vector<float> &pulse,  float amplitude, float phase, float lag) const;
    void AddGaussianNoise( std::vector<float> &pulse, float noiseMean, float noiseStddev) const;


    /*! Number of samples to be generated */
    int m_nsamples;
    int m_startSamplingBC;
    int m_shaperZeroIndex;
    float m_pedestal;
    float m_deformationMean;
    float m_deformationStd;
    float m_samplingRate;
    float m_shaperResolution;
    float m_noiseMean;    
    float m_noiseStd;    

    std::vector<float> m_shaper;
    std::vector<float> m_timeSeries;
    
    /*! The shaper configuration path */
    std::string m_shaperFile;
    /*! Output level message */
    int m_outputLevel;
    /*! Random generator */
    mutable TRandom3 m_rng;
};

#endif




