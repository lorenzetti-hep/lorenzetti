
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


                                                                                                               \




