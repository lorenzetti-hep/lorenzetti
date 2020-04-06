#ifndef PulseGenerator_h
#define PulseGenerator_h

#include "GaugiKernel/StatusCode.h"
#include "ICaloCellTool.h"
#include "TPulseGenerator.h"



class PulseGenerator : public ICaloCellTool
{

  public:
    /** Constructor **/
    PulseGenerator( std::string name );
    virtual ~PulseGenerator();
    
    virtual StatusCode initialize() override;
    virtual StatusCode finalize() override;

    virtual StatusCode executeTool( xAOD::CaloCell * ) const override;


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




