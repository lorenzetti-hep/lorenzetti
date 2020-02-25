#ifndef CaloClusterMaker_h
#define CaloClusterMaker_h

/** simulator libs **/
#include "core/StatusCode.h"
#include "core/EventContext.h"
#include "core/AlgTool.h"

/** standard libs **/
#include <string>


class CaloClusterMaker : public AlgTool
{

  public:
    /** Constructor **/
    CaloClusterMaker( std::string );
    /** Destructor **/
    virtual ~CaloClusterMaker();
    /** initialize the object **/
    virtual StatusCode initialize() override;
    /** pre execute the event before step action **/
    virtual StatusCode pre_execute( EventContext *ctx ) override;
    /** post execute the event after step action **/
    virtual StatusCode post_execute( EventContext *ctx ) override;
    /** fill histograms after post execute **/
    virtual StatusCode fill( EventContext *ctx ) override;


    PRIMITIVE_SETTER_AND_GETTER( float, m_etaWindow, setEtaWindow, etaWindow )
    PRIMITIVE_SETTER_AND_GETTER( float, m_phiWindow, setPhiWindow, phiWindow )
    PRIMITIVE_SETTER_AND_GETTER( float, m_energyThreshold, setEnergyThreshold, energyThreshold )
    

  private:
    
    // eta window
    float m_etaWindow;
    // phi window
    float m_phiWindow;
    // the energy threshold to accept a RoI center
    float m_energyThreshold;

};

#endif




