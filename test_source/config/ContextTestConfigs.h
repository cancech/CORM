/*
 * ContextTestConfigs.h
 *
 *  Created on: Aug. 2, 2020
 *      Author: Jaroslav Bosak
 */

#ifndef CONFIG_CONTEXTTESTCONFIGS_H_
#define CONFIG_CONTEXTTESTCONFIGS_H_

#include "Configuration.h"

// Configuration which required a couple of resources, which are expected to be missing.
CONFIGURATION(SingleConfigMissingResourcesTestConfig)

RESOURCES(SingleConfigMissingResourcesTestConfig,
		(int, missingIntValue),
		(std::string&, missingStringReference)
)

#endif /* CONFIG_CONTEXTTESTCONFIGS_H_ */
