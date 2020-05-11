#ifndef OptimalFilter_h
#define OptimalFilter_h

#include "CaloTool.h"



class OptimalFilter : public CaloTool
{

  public:
    /** Constructor **/
    OptimalFilter( std::string name );
    virtual ~OptimalFilter();
    virtual StatusCode initialize() override;
    virtual StatusCode finalize() override;
    virtual StatusCode executeTool( const xAOD::EventInfo *, xAOD::RawCell * ) const override;
    virtual StatusCode executeTool( const xAOD::EventInfo *, xAOD::CaloCell * ) const override;
    virtual StatusCode executeTool( const xAOD::EventInfo *, xAOD::CaloCluster * ) const override;
    virtual StatusCode executeTool( const xAOD::EventInfo *, xAOD::TruthParticle * ) const override;


  private:

    /*! optimal filter weights */
    std::vector<float> m_ofweights; 
};

#endif




