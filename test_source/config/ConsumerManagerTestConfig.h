/*
 * ConsumerManagerTestConfig.h
 *
 *  Created on: Jul. 28, 2020
 *      Author: Jaroslav Bosak
 */

#ifndef CONFIG_CONSUMERMANAGERTESTCONFIG_H_
#define CONFIG_CONSUMERMANAGERTESTCONFIG_H_

#include "Configuration.h"
#include "DummyClass.h"

CONFIGURATION(ConsumerManagerTestConfig)

RESOURCES(ConsumerManagerTestConfig,
		(DummyClass*, providerManagerDummySingleton),
		(DummyClass, providerConsumerDummyFactory)
)



#endif /* CONFIG_CONSUMERMANAGERTESTCONFIG_H_ */
