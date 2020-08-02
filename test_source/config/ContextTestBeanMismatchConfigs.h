/*
 * ContextTestBeanMismatchConfigs.h
 *
 *  Created on: Aug. 2, 2020
 *      Author: Jaroslav Bosak
 */

#ifndef CONFIG_CONTEXTTESTBEANMISMATCHCONFIGS_H_
#define CONFIG_CONTEXTTESTBEANMISMATCHCONFIGS_H_

#include "Configuration.h"
#include "DummyClass.h"

// Provider config which provides a single DummyClass instance
CONFIGURATION(DummyBeanProviderTestConfig)

	void provideBeans() {
		beanManager->registerBean<DummyClass*>("BeanName");
	}


CONFIGURATION_END_NO_RESOURCE(DummyBeanProviderTestConfig)



// Consumer config which tries to load the bean as an int
CONFIGURATION(DummyBeanAsIntConsumerTestConfig)

RESOURCES(DummyBeanAsIntConsumerTestConfig,
		(int, BeanName)
)



// Consumer config which tries to load the bean pointer as a scalar
CONFIGURATION(PtrAsScalarConsumerTestConfig)

RESOURCES(PtrAsScalarConsumerTestConfig,
		(DummyClass, BeanName)
)



#endif /* CONFIG_CONTEXTTESTBEANMISMATCHCONFIGS_H_ */