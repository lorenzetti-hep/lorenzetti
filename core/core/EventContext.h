#ifndef EventContext_h
#define EventContext_h

/** simulator libs **/
#include "core/macros.h"
#include "core/StatusCode.h"
#include "CaloCluster/CaloClusterContainer.h"
#include "CaloCell/CaloCellContainer.h"
#include "EventInfo/EventInfoContainer.h"
#include "TruthParticle/TruthParticleContainer.h"

/** geant 4 libs **/
#include "G4Step.hh"



// Used only in event context class

#define PRIMITIVE_ATTACH_AND_RETRIEVE( CONTAINER, VAR )                                                             \
bool attach( CONTAINER * ptr, std::string key="", bool force=false )                                                \
{                                                                                                                   \
  if ( (VAR.find(key) == VAR.end())  || force) {                                                                    \
    VAR[key] = ptr;                                                                                                 \
    return true;                                                                                                    \
  }                                                                                                                 \
  MSG_ERROR( "The key exist into the event context. Can not attach this feature using key: " << key );              \
  return false;                                                                                                     \
}                                                                                                                   \
bool retrieve( CONTAINER*& ptr, std::string key="" )                                                                \
{                                                                                                                   \
  if ( VAR.find(key) != VAR.end() ) {                                                                               \
    ptr = VAR[key];                                                                                                 \
    return true;                                                                                                    \
  }                                                                                                                 \
  MSG_ERROR( "The key does not exist into the event context. Can not retrieve this feature using key: " << key );   \
  return false;                                                                                                     \
}    




class EventContext
{
  public:

    /** Constructor **/
    EventContext();
    /** Destructor **/
    ~EventContext();

    StatusCode initialize();

    StatusCode finalize();

    PRIMITIVE_ATTACH_AND_RETRIEVE( const G4Step                       , m_stepHandleKey         );
    PRIMITIVE_ATTACH_AND_RETRIEVE( const xAOD::CaloCellContainer      , m_caloCellHandleKey     );
    PRIMITIVE_ATTACH_AND_RETRIEVE( const xAOD::CaloClusterContainer   , m_caloClusterHandleKey  );
    PRIMITIVE_ATTACH_AND_RETRIEVE( const xAOD::EventInfoContainer     , m_eventInfoHandleKey    );
    PRIMITIVE_ATTACH_AND_RETRIEVE( const xAOD::TruthParticleContainer , m_truthHandleKey        );

  
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
    std::map< std::string, const xAOD::TruthParticleContainer*> m_truthHandleKey;

};

  
  

#endif

