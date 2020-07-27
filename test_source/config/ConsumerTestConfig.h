/*
 * ConsumerTestConfig.h
 *
 *  Created on: Jul. 27, 2020
 *      Author: Jaroslav Bosak
 */

#ifndef CONFIG_CONSUMERTESTCONFIG_H_
#define CONFIG_CONSUMERTESTCONFIG_H_

#include "Configuration.h"
#include "DummyClass.h"

CONFIGURATION(ConsumerTestConfig)

	int* getProviderSomeIntValue() {
		return &providerSomeIntValue;
	}

	DummyClass* getProviderDummyClassSingleton() {
		return providerDummyClassSingleton;
	}

RESOURCES(ConsumerTestConfig,
		(DummyClass, providerDummyClassFactory),
		(DummyClass*, providerDummyClassSingleton),
		(int&, providerSomeIntValue)
)



#endif /* CONFIG_CONSUMERTESTCONFIG_H_ */
