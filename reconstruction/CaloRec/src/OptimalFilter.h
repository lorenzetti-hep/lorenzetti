#ifndef OptimalFilter_h
#define OptimalFilter_h

#include "ICaloCellTool.h"



class OptimalFilter : public ICaloCellTool
{

  public:
    /** Constructor **/
    OptimalFilter( std::string name, int msglevel=1 );
    virtual ~OptimalFilter();
    virtual StatusCode initialize() override;
    virtual StatusCode finalize() override;
    virtual StatusCode executeTool( xAOD::CaloCell * ) const override;


  private:
    std::vector<float> m_ofweights; 
};

#endif




