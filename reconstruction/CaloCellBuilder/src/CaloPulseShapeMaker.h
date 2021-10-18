#ifndef CaloPulseShapeMaker_h
#define CaloPulseShapeMaker_h

#include "GaugiKernel/StatusCode.h"
#include "GaugiKernel/AlgTool.h"
#include "GaugiKernel/EDM.h"
#include "TRandom3.h"


class CaloPulseShapeMaker : public Gaugi::AlgTool
{

  public:
    /** Constructor **/
    CaloPulseShapeMaker( std::string name );
    virtual ~CaloPulseShapeMaker();
    
    virtual StatusCode initialize() override;
    virtual StatusCode execute( const xAOD::EventInfo *, Gaugi::EDM * ) const override;
    virtual StatusCode finalize() override;

  private:

    void ReadShaper( std::string );

    /*! Index where t = 0 */
    unsigned int m_origin;
    /*! The shaper time resolution in ns */
    float m_resolution;
    /*! The shaper configuration path */
    std::string m_shaperFile;
    /*! The shaper values */
    std::vector<std::pair<float,float>> m_pulseShape;
    /*! Output level message */
    int m_outputLevel;
};

#endif
