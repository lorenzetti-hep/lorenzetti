#ifndef PulseGenerator_h
#define PulseGenerator_h

#include "GaugiKernel/StatusCode.h"
#include "ICaloCellTool.h"
#include "TPulseGenerator.h"



class PulseGenerator : public ICaloCellTool
{

  public:
    /** Constructor **/
    PulseGenerator( std::string name, int msglevel=0 );
    virtual ~PulseGenerator();
    
    virtual StatusCode initialize() override;
    virtual StatusCode finalize() override;

    virtual StatusCode executeTool( xAOD::CaloCell * ) const override;


  private:
 
    /*! Number of samples to be generated */
    int m_nsamples;
    /*! The shaper configuration path */
    std::string m_card;
    /*! Pulse generator */
    CPK::TPulseGenerator  *m_pulseGenerator;
};

#endif




