#ifndef OptimalFilter_h
#define OptimalFilter_h

#include <vector>
#include <TMatrixD.h>
#include "GaugiKernel/StatusCode.h"
#include "GaugiKernel/AlgTool.h"
#include "GaugiKernel/EDM.h"
#include "CaloCell/CaloDetDescriptor.h"


class OptimalFilter : public Gaugi::AlgTool
{

  public:
    enum Restrictions {
      SINGLE_RESTRICTION=1,
      DOUBLE_RESTRICTION=2
    };

    /** Constructor **/
    OptimalFilter( std::string name );
    virtual ~OptimalFilter();
    virtual StatusCode initialize() override;
    virtual StatusCode finalize() override;
    virtual StatusCode execute( const xAOD::EventInfo *, Gaugi::EDM * ) const override;

  private:

    /*! optimal filter weights */
    float Filter( const std::vector<float> &pulse , const std::vector<float> &coeffs ) const;
    const std::vector<float> CalculateOptCoeffs( const unsigned int filterSize, const xAOD::CaloDetDescriptor* cell ) const;
    const TMatrixD* GetNoiseCovarianceMatrix( unsigned int filterSize ) const;
    StatusCode ReadCalibrationDataset();

    // filter design properties
    bool               m_useDoubleRestriction;

    // pulse information
    double             m_samplingRate;
    double             m_startSamplingBC;

    // calibration properties
    bool               m_doCalibration;
    std::string        m_calibrationDatasetFile;
    TMatrixD*          m_calibrationDataset;

    // static mode
    std::vector<float> m_staticCoeffs;
};

#endif
