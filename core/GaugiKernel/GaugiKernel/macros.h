/// @param TYPE The (primitive) type name
/// @param VAR The name of the auxiliary variable
/// @param SETTER The name of the "setter function"
/// @param GETTER The name of the "getter function"


#define PRIMITIVE_SETTER_AND_GETTER(TYPE, VAR, SETTER, GETTER)\
TYPE GETTER() const {                                     \
return VAR;                              \
}                                                           \
void SETTER( TYPE value ) {                           \
VAR=value;     \
}\



