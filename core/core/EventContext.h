#ifndef EventContext_h
#define EventContext_h

/** simulator libs **/
#include "core/macros.h"
#include "CaloCluster/CaloClusterContainer.h"
#include "CaloCell/CaloCellContainer.h"
#include "EventInfo/EventInfoContainer.h"
#include "Truth/TruthContainer.h"

/** geant 4 libs **/
#include "G4Step.hh"






class EventContext
{
  public:

    /** Constructor **/
    EventContext();
    /** Destructor **/
    ~EventContext();


    PRIMITIVE_ATTACH_AND_RETRIEVE( const xAOD::CaloCellContainer    , m_caloCellHandleKey     );
    PRIMITIVE_ATTACH_AND_RETRIEVE( const xAOD::CaloClusterContainer , m_caloClusterHandleKey  );
    PRIMITIVE_ATTACH_AND_RETRIEVE( const xAOD::EventInfoContainer   , m_eventInfoHandleKey    );
    PRIMITIVE_ATTACH_AND_RETRIEVE( const xAOD::truthContainer       , m_truthHandleKey        );

  
  private:
    // G4Step point from geant
    std::map< std::string, const G4Step*> m_stepHandleKey;
    // calo cell containers
    std::map< std::string, const xAOD::CaloCellContainer*> m_caloCellHandleKey;
    // calo cluster containers
    std::map< std::string, const xAOD::CaloClusterContainer*> m_caloClusterHandleKey;
    // Event info containers
    std::map< std::string, const xAOD::EventInfoContainer*>  m_eventInfoHandleKey;
    // truth containers
    std::map< std::string, const xAOD::TruthContainer*> m_truthHandleKey;

};

  
  

#endif

