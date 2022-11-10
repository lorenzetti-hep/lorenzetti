#ifndef ConstrainedOptimalFilter_h
#define ConstrainedOptimalFilter_h

#include "GaugiKernel/AlgTool.h"
#include "GaugiKernel/EDM.h"



class ConstrainedOptimalFilter : public Gaugi::AlgTool
{

  public:
    /** Constructor **/
    ConstrainedOptimalFilter( std::string name );
    virtual ~ConstrainedOptimalFilter();
    virtual StatusCode initialize() override;
    virtual StatusCode finalize() override;

    virtual StatusCode execute( SG::EventContext &ctx, Gaugi::EDM * ) const override;

  private:

    /*! optimal filter weights */
    std::vector<float> m_ofweights; 
};

#endif




