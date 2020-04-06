#ifndef OptimalFilter_h
#define OptimalFilter_h

#include "CaloRecTool.h"



class OptimalFilter : public CaloRecTool
{

  public:
    /** Constructor **/
    OptimalFilter( std::string name );
    virtual ~OptimalFilter();
    virtual StatusCode initialize() override;
    virtual StatusCode finalize() override;
    virtual StatusCode executeTool( xAOD::CaloCell * ) const override;


  private:

    /*! optimal filter weights */
    std::vector<float> m_ofweights; 
};

#endif




