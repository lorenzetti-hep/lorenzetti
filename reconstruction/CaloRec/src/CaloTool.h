#ifndef CaloTool_h
#define CaloTool_h

#include "GaugiKernel/AlgTool.h"
#include "CaloCell/RawCell.h"
#include "CaloCell/CaloCell.h"
#include "CaloCluster/CaloCluster.h"
#include "TruthParticle/TruthParticle.h"


class CaloTool : public Gaugi::AlgTool
{

  public:
    /*! Constructor */
    CaloTool() : Gaugi::AlgTool(){};
    
    /*! execute call */
    virtual StatusCode executeTool( xAOD::RawCell * ) const = 0;
    virtual StatusCode executeTool( xAOD::CaloCell * ) const = 0;
    virtual StatusCode executeTool( xAOD::CaloCluster * ) const = 0;
    virtual StatusCode executeTool( xAOD::TruthParticle * ) const = 0;

};

#endif




