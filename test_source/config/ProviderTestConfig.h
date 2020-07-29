/*
 * ProviderTestConfig.h
 *
 *  Created on: Jul. 27, 2020
 *      Author: Jaroslav Bosak
 */

#ifndef CONFIG_PROVIDERTESTCONFIG_H_
#define CONFIG_PROVIDERTESTCONFIG_H_

#include "Configuration.h"
#include "DummyClass.h"

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

#endif /* CONFIG_PROVIDERTESTCONFIG_H_ */
