/*
 * NoResourceConfig.h
 *
 *  Created on: Jul. 27, 2020
 *      Author: Jaroslav Bosak
 */

#ifndef CONFIG_NORESOURCETESTCONFIG_H_
#define CONFIG_NORESOURCETESTCONFIG_H_

#include "Configuration.h"

CONFIGURATION(NoResourceTestConfig)

	int numTimesPostConstructCalled = 0;
	int numTimesProvideBeansCalled = 0;

	virtual void postInit() {
		numTimesPostConstructCalled++;
	}

	virtual void provideBeans() {
		numTimesProvideBeansCalled++;
	}

CONFIGURATION_END_NO_RESOURCE(NoResourceTestConfig)

#endif /* CONFIG_NORESOURCETESTCONFIG_H_ */
