/*
 * SimpleCormConfig2.h
 *
 *  Created on: Jul. 4, 2020
 *      Author: Jaroslav Bosak
 *
 * Test configuration to provide some beans
 */

#ifndef SIMPLECORMCONFIG2_H_
#define SIMPLECORMCONFIG2_H_

#include "SimpleCorm.h"

/*
 * Provides intBean1 for testing
 */
BEAN(int, intBean1) {
	return 123;
}

/*
 * Provides dblBean for testing
 */
BEAN(double*, dblBean) {
	return new double(1.111);
}

#endif /* SIMPLECORMCONFIG2_H_ */
