#ifndef CaloTool_h
#define CaloTool_h

#include "GaugiKernel/AlgTool.h"
#include "CaloCell/RawCell.h"
#include "CaloCell/CaloCell.h"
#include "CaloCluster/CaloCluster.h"
#include "TruthParticle/TruthParticle.h"
#include "EventInfo/EventInfo.h"


class CaloTool : public Gaugi::AlgTool
{

  public:
    /*! Constructor */
    CaloTool() : Gaugi::AlgTool(){};
    
    /*! execute call */
    virtual StatusCode executeTool( const xAOD::EventInfo *, xAOD::RawCell * ) const = 0;
    virtual StatusCode executeTool( const xAOD::EventInfo *, xAOD::CaloCell * ) const = 0;
    virtual StatusCode executeTool( const xAOD::EventInfo *, xAOD::CaloCluster * ) const = 0;
    virtual StatusCode executeTool( const xAOD::EventInfo *, xAOD::TruthParticle * ) const = 0;

};

#endif




