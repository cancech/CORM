#ifndef SIMPLECORMADDER_H_
#define SIMPLECORMADDER_H_

#include <iostream>

#include "simple/SimpleCorm.h"

// The externs have to be here in order to allow compilation to complete.
// Only way to remove this need is to create a separate CORM preprocessor
// which would generate these lines.
BEAN(int, intBean1);
BEAN(int, intBean2);

/*
 * Test class to be used to verify that a class can pull in beans as a resource
 */
class SimpleCormAdder {
public:
	int add() { return intBean1 + intBean2; }

private:
	// The members that are initialized with a resource
	RESOURCE_MEMBER(int, intBean1);
	RESOURCE_MEMBER(int, intBean2);
};



#endif /* SIMPLECORMADDER_H_ */
