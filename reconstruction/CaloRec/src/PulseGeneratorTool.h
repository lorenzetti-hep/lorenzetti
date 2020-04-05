#ifndef PulseGeneratorTool_h
#define PulseGeneratorTool_h

#include "GaugiKernel/StatusCode.h"
#include "ICaloCellTool.h"
#include "TPulseGenerator.h"



class PulseGeneratorTool : public ICaloCellTool
{

  public:
    /** Constructor **/
    PulseGeneratorTool( std::string );
    virtual ~PulseGeneratorTool();
    
    virtual StatusCode initialize() override;
    virtual StatusCode finalize() override;

    virtual StatusCode executeTool( CaloCell * ) const override;


  private:
 
    /*! Number of samples to be generated */
    int m_nsamples;
    /*! The shaper configuration path */
    std::string m_cardPath;
    /*! Pulse generator */
    CPK::TPulseGenerator  *m_pulseGenerator;
};

#endif




