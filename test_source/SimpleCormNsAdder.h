/*
 * SimpleCormNsAdder.h
 *
 *  Created on: Jul. 5, 2020
 *      Author: Jaroslav Bosak
 */

#ifndef SIMPLECORMNSADDER_H_
#define SIMPLECORMNSADDER_H_

#include <iostream>

#include "SimpleCorm.h"

// The externs have to be here in order to allow compilation to complete.
// Only way to remove this need is to create a separate CORM preprocessor
// which would generate these lines.
namespace testing {
	BEAN(int, nsIntBean1);
	BEAN(int, nsIntBean2);
}

/*
 * Test class to be used to verify that a class can pull in beans as a resource
 */
class SimpleCormNsAdder {
public:
	int add() { return nsIntBean1 + nsIntBean2; }

private:
	// The members that are initialized with a resource
	RESOURCE_MEMBER_NS(int, testing, nsIntBean1);
	RESOURCE_MEMBER_NS(int, testing, nsIntBean2);
};



#endif /* SIMPLECORMNSADDER_H_ */
