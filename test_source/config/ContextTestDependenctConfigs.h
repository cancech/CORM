#ifndef CONFIG_CONTEXTTESTDEPENDENCTCONFIGS_H_
#define CONFIG_CONTEXTTESTDEPENDENCTCONFIGS_H_

#include "Configuration.h"
#include "DummyClass.h"


// Config which provides a single bean
CONFIGURATION(ProviderManagerTestConfig)

	BEANS(
			(BEAN, DummyClass*, "providerManagerDummySingleton")
	)

END_CONFIGURATION



// Config which requires a single resource from ProviderManagerTestConfig and provides a single bean
CONFIGURATION(ProviderConsumerManagerTestConfig)

	BEANS(
			(BEAN, DummyClass, corm::FactoryBeanCreator<DummyClass>, "providerConsumerDummyFactory")
	)

	RESOURCES(ProviderConsumerManagerTestConfig,
			(DummyClass*, providerManagerDummySingleton)
	)

END_CONFIGURATION



// Config which requires two resources provided by ProviderManagerTestConfig and ProviderConsumerManagerTestConfig
CONFIGURATION(ConsumerManagerTestConfig)

	RESOURCES(ConsumerManagerTestConfig,
			(DummyClass*, providerManagerDummySingleton),
			(DummyClass, providerConsumerDummyFactory)
	)

END_CONFIGURATION


// Config which lists the above configs as dependencies and verifies that the beans were properly loaded.
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

END_CONFIGURATION

#endif /* CONFIG_CONTEXTTESTDEPENDENCTCONFIGS_H_ */
