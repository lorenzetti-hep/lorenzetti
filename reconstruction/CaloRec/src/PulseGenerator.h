#ifndef PulseGenerator_h
#define PulseGenerator_h

#include "GaugiKernel/StatusCode.h"
#include "CaloTool.h"
#include "TPulseGenerator.h"



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
 
    /*! Number of samples to be generated */
    int m_nsamples;
    /*! The shaper configuration path */
    std::string m_shaperFile;
    /*! Pulse generator */
    CPK::TPulseGenerator  *m_pulseGenerator;
    /*! Output level message */
    int m_outputLevel;
};

#endif




