/*
 * ConfigurationTestConfigs.h
 *
 *  Created on: Aug. 2, 2020
 *      Author: Jaroslav Bosak
 *
 *
 * Provides the various configuration files that ConfigurationTest.cpp requires for its tests.
 */

#ifndef CONFIG_CONFIGURATIONTESTCONFIGS_H_
#define CONFIG_CONFIGURATIONTESTCONFIGS_H_

#include "Configuration.h"
#include "DummyClass.h"

// Configuration that is expected to not have its resources fulfilled.
CONFIGURATION(MissingResourcesTestConfig)

RESOURCES(MissingResourcesTestConfig,
		(int, missingIntValue),
		(std::string&, missingStringReference),
		(DummyClass*, missingDummyPointer)
)



// Configuration that required no resources nor provides any beans
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



// Configuration which requires no resources but provides some beans
CONFIGURATION(ProviderTestConfig)

	virtual void postInit() {
		someIntValue = 56789;
	}

	virtual void provideBeans() {
		beanManager->registerBean<DummyClass, corm::FactoryBeanCreator<DummyClass>>("providerDummyClassFactory");
		beanManager->registerBean<DummyClass*>("providerDummyClassSingleton");
		beanManager->registerBeanInstance<int&>("providerSomeIntValue", someIntValue);
	}

	int* getSomeIntValuePtr() {
		return &someIntValue;
	}

private:
	int someIntValue = 0;

CONFIGURATION_END_NO_RESOURCE(ProviderTestConfig)



// Configuration that provides no beans but pulls in all of the resources provided by ProviderTestConfig
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

#endif /* CONFIG_CONFIGURATIONTESTCONFIGS_H_ */