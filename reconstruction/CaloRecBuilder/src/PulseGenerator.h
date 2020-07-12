#ifndef PulseGenerator_h
#define PulseGenerator_h

#include "GaugiKernel/StatusCode.h"
#include "CaloTool.h"


class PulseGenerator : public CaloTool
{

  public:
    /** Constructor **/
    PulseGenerator( std::string name );
    virtual ~PulseGenerator();
    
    virtual StatusCode initialize() override;
    virtual StatusCode finalize() override;

    virtual StatusCode executeTool( const xAOD::EventInfo *, xAOD::RawCell * ) const override;
    virtual StatusCode executeTool( const xAOD::EventInfo *, xAOD::CaloCell * ) const override;
    virtual StatusCode executeTool( const xAOD::EventInfo *, xAOD::CaloCluster * ) const override;
    virtual StatusCode executeTool( const xAOD::EventInfo *, xAOD::TruthParticle * ) const override;



  private:

    void ReadShaper( std::string );
    void GenerateDeterministicPulse(  std::vector<float> &pulse,  float amplitude, float phase, float lag) const;
    void AddGaussianNoise( std::vector<float> &pulse, float noiseMean, float noiseStddev) const;


    /*! Number of samples to be generated */
    int m_nsamples;
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
};

#endif




