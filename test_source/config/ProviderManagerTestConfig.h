/*
 * Provider1ManagerTestConfig.h
 *
 *  Created on: Jul. 28, 2020
 *      Author: Jaroslav Bosak
 */

#ifndef CONFIG_PROVIDERMANAGERTESTCONFIG_H_
#define CONFIG_PROVIDERMANAGERTESTCONFIG_H_

#include "Configuration.h"
#include "DummyClass.h"

CONFIGURATION(ProviderManagerTestConfig)

	virtual void provideBeans() {
		beanManager->registerBean<DummyClass*>("providerManagerDummySingleton");
	}

CONFIGURATION_END_NO_RESOURCE(ProviderManagerTestConfig)

#endif /* CONFIG_PROVIDERMANAGERTESTCONFIG_H_ */
