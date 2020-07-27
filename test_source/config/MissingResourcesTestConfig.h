/*
 * Config1.h
 *
 *  Created on: Jul. 25, 2020
 *      Author: Jaroslav Bosak
 */

#ifndef CONFIG_MISSINGRESOURCESTESTCONFIG_H_
#define CONFIG_MISSINGRESOURCESTESTCONFIG_H_

#include "Configuration.h"
#include "DummyClass.h"

CONFIGURATION(MissingResourcesTestConfig)

RESOURCES(MissingResourcesTestConfig,
		(int, missingIntValue),
		(std::string&, missingStringReference),
		(DummyClass*, missingDummyPointer)
)

#endif /* CONFIG_MISSINGRESOURCESTESTCONFIG_H_ */
