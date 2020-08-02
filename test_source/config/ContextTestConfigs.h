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



// Configuration which provides a single bean
CONFIGURATION(ProvideBean1Config)

	void provideBeans() {
		beanManager->registerBean<int&>("DuplicateBean");
	}

CONFIGURATION_END_NO_RESOURCE(ProvideBean1Config)

// Configuration which provides a single bean, same name as ProvideBean1Config
CONFIGURATION(ProvideBean2Config)

	void provideBeans() {
		beanManager->registerBean<double&>("DuplicateBean");
	}

CONFIGURATION_END_NO_RESOURCE(ProvideBean2Config)

#endif /* CONFIG_CONTEXTTESTCONFIGS_H_ */
