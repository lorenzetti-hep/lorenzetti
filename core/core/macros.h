
/// @param TYPE The (primitive) type name
/// @param VAR The name of the auxiliary variable
/// @param SETTER The name of the "setter function"
/// @param GETTER The name of the "getter function"
#define PRIMITIVE_SETTER_AND_GETTER(TYPE, VAR, SETTER, GETTER)\
	TYPE GETTER() const {                                       \
		return VAR;                                						    \
	}                                                           \
	void SETTER( TYPE value ) {                           	    \
		VAR=value;    																				    \
	}																												    \


#define MSG_INFO( message ) \
  G4cout << "[INFO]     " << message << G4endl; \

#define MSG_WARNING( message ) \
  G4cout << "[WARNING]  " << message << G4endl; \

#define MSG_ERROR( message ) \
  G4cout << "[ERROR]  " << message << G4endl; \

#define MSG_FATAL( message ) \
  G4cout << "[FATAL]  " << message << G4endl; \




// Used only in event context class

#define PRIMITIVE_ATTACH_AND_RETRIEVE( CONTAINER, VAR )                                                             \
void attach( const CONTAINER * ptr, std::string key="" )                                                            \
{                                                                                                                   \
  if ( VAR.find(key) == VAR.end() ) {                                                                               \
    VAR[key] = ptr;                                                                                                 \
    return true;                                                                                                    \
  }                                                                                                                 \
  MSG_ERROR( "The key exist into the event context. Can not attach this feature using key: " << key );              \
  return false;                                                                                                     \
}                                                                                                                   \
bool retrieve( const CONTAINER*& ptr, std::string key="" )                                                          \
{                                                                                                                   \
  if ( VAR.find(key) == VAR.end() ) {                                                                               \
    ptr = VAR[key];                                                                                                 \
    return true;                                                                                                    \
  }                                                                                                                 \
  MSG_ERROR( "The key does not exist into the event context. Can not retrieve this feature using key: " << key );   \
  return false;                                                                                                     \
}                                                                                                                   \




