#ifndef "CaloClusterMaker_h"
#define "CaloClusterMaker_h"

/** simulator libs **/
#include "StatusCode.h"
#include "EventContext.h"
#include "AlgTool.h"

/** standard libs **/
#include <string>


class CaloClusterMaker : public AlgTool
{

  public:
    /** Constructor **/
    virtual CaloClusterMaker( std::string & );
    /** Destructor **/
    virtual ~CaloCluster();
    /** initialize the object **/
    virtual StatusCode initialize() override;
    /** pre execute the event before step action **/
    virtual StatusCode pre_execute( EventContext *ctx ) override;
    /** post execute the event after step action **/
    virtual StatusCode post_execute( EventContext *ctx ) override;
    /** fill histograms after post execute **/
    virtual StatusCode fill( EventContext *ctx ) override;

  private:
    
    // eta window
    float m_etaWindow;
    // phi window
    float m_phiWindow;
    // the energy threshold to accept a RoI center
    float m_energyThreshold;

};






