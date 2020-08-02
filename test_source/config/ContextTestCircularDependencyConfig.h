/*
 * ContextTestCircularDependencyConfig.h
 *
 *  Created on: Aug. 2, 2020
 *      Author: Jaroslav Bosak
 */

#ifndef CONFIG_CONTEXTTESTCIRCULARDEPENDENCYCONFIG_H_
#define CONFIG_CONTEXTTESTCIRCULARDEPENDENCYCONFIG_H_

#include "Configuration.h"

// Configuration which requires a resource from CircularDep2TestConfig and provides a bean
// CircularDep2TestConfig requires
CONFIGURATION(CircularDep1TestConfig)

	BEANS(
			(BEAN, double&, "circularBean2")
	)

	RESOURCES(CircularDep1TestConfig,
			(int&, circularBean1)
	)

END_CONFIGURATION


// Configuration which requires a resource from CircularDep1TestConfig and provides a bean
// CircularDep1TestConfig requires
CONFIGURATION(CircularDep2TestConfig)

	BEANS(
			(BEAN, int&, "circularBean1")
	)

	RESOURCES(CircularDep2TestConfig,
			(double&, circularBean2)
	)

END_CONFIGURATION

#endif /* CONFIG_CONTEXTTESTCIRCULARDEPENDENCYCONFIG_H_ */
