/*
 * Bean.h
 *
 *  Created on: Jul. 4, 2020
 *      Author: Jaroslav Bosak
 */

#ifndef SIMPLECORM_H_
#define SIMPLECORM_H_

// Generates the method signature for the resource provider
#define BEAN(Type, Name) Type provide_##Name()

// Generates the bean member instance field within a class definition
#define RESOURCE_MEMBER(Type, Name) Type Name = provide_##Name()
// Generates the bean outside of a class definition (includes the extern for the resource)
#define RESOURCE(Type, Name) extern BEAN(Type, Name); RESOURCE_MEMBER(Type, Name)

#endif /* SIMPLECORM_H_ */
