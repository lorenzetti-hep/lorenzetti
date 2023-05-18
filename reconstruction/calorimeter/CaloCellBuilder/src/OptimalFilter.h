#ifndef OptimalFilter_h
#define OptimalFilter_h

#include "GaugiKernel/AlgTool.h"
#include "GaugiKernel/EDM.h"



class OptimalFilter : public Gaugi::AlgTool
{

  public:
    /** Constructor **/
    OptimalFilter( std::string name );
    virtual ~OptimalFilter();
    virtual StatusCode initialize() override;
    virtual StatusCode finalize() override;

    virtual StatusCode execute( SG::EventContext &ctx, Gaugi::EDM * ) const override;

  private:

    /*! optimal filter weights */
    std::vector<float> m_ofweights; 
};

#endif




