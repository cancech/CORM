/*
 * ProviderConsumerManagerTestConfig.h
 *
 *  Created on: Jul. 28, 2020
 *      Author: Jaroslav Bosak
 */

#ifndef CONFIG_PROVIDERCONSUMERMANAGERTESTCONFIG_H_
#define CONFIG_PROVIDERCONSUMERMANAGERTESTCONFIG_H_

#include "Configuration.h"
#include "DummyClass.h"

CONFIGURATION(ProviderConsumerManagerTestConfig)

	virtual void provideBeans() {
		beanManager->registerBean<DummyClass, corm::FactoryBeanCreator<DummyClass>>("providerConsumerDummyFactory");
	}

RESOURCES(ProviderConsumerManagerTestConfig,
		(DummyClass*, providerManagerDummySingleton)
)

#endif /* CONFIG_PROVIDERCONSUMERMANAGERTESTCONFIG_H_ */
