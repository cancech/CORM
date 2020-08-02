#ifndef SIMPLECORM_H_
#define SIMPLECORM_H_

// Helper to generate the providing method name to avoid repetition
#define MTHD_NAME(Name) provide_##Name()

// Generates the method signature for the resource provider
#define BEAN(Type, Name) Type MTHD_NAME(Name)

// Generates the bean member instance field within a class definition
#define RESOURCE_MEMBER(Type, Name) Type Name = MTHD_NAME(Name)
#define RESOURCE_MEMBER_NS(Type, Namespace, Name) Type Name = Namespace::MTHD_NAME(Name)
// Generates the bean outside of a class definition (includes the extern for the resource)
#define RESOURCE(Type, Name) extern BEAN(Type, Name); RESOURCE_MEMBER(Type, Name)
#define RESOURCE_NS(Type, Namespace, Name) extern Type Namespace::MTHD_NAME(Name); \
	RESOURCE_MEMBER_NS(Type, Namespace, Name)

#endif /* SIMPLECORM_H_ */
