#ifndef CaloRecTool_h
#define CaloRecTool_h

#include "GaugiKernel/AlgTool.h"
#include "CaloCell/CaloCell.h"
#include "CaloCluster/CaloCluster.h"
#include "TruthParticle/TruthParticle.h"


class CaloRecTool : public Gaugi::AlgTool
{

  public:
    /*! Constructor */
    CaloRecTool( std::string name ) : Gaugi::AlgTool(name){};
    
    /*! execute call */
    virtual StatusCode executeTool( xAOD::CaloCell * ) const = 0;
    virtual StatusCode executeTool( xAOD::CaloCluster * ) const = 0;
    virtual StatusCode executeTool( xAOD::TruthParticle * ) const = 0;

};

#endif




