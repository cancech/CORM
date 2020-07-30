/*
 * ConfigWithDeps.h
 *
 *  Created on: Jul. 30, 2020
 *      Author: Jaroslav Bosak
 */

#ifndef CONFIG_CONFIGWITHDEPS_H_
#define CONFIG_CONFIGWITHDEPS_H_

#include <cassert>
#include "Configuration.h"

#include "ConsumerManagerTestConfig.h"
#include "ProviderConsumerManagerTestConfig.h"
#include "ProviderManagerTestConfig.h"
#include "DummyClass.h"

CONFIGURATION(ConfigWithDeps)

	DEPENDENCIES(ConsumerManagerTestConfig, ProviderConsumerManagerTestConfig, ProviderManagerTestConfig)

	virtual void postInit() {
		assert(providerManagerDummySingleton != NULL);
		assert(providerManagerDummySingleton->getValue() == providerConsumerDummyFactory.getValue());
	}

	RESOURCES(ConfigWithDeps,
		(DummyClass*, providerManagerDummySingleton),
		(DummyClass, providerConsumerDummyFactory)
)



#endif /* CONFIG_CONFIGWITHDEPS_H_ */
