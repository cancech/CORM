#ifndef PERFORMANCE_RUNTIMECONFIG_H_
#define PERFORMANCE_RUNTIMECONFIG_H_

#include "corm/Configuration.h"
#include "DummyClass.h"

#include <cassert>

// Configuration which provides int 1 and 2
CONFIGURATION(ProvideSomeIntsConfig)

	BEANS(
			(BEAN_INSTANCE, int&, int1),
			(BEAN_INSTANCE, int&, int2)
	)

private:
	int int1 = 123;
	int int2 = 234;

END_CONFIGURATION

// Configuration which provides int 3
CONFIGURATION(ProvideSomeMoreIntsConfig)

	BEANS(
			(BEAN_INSTANCE, int&, int3)
	)

private:
	int int3 = 456;

END_CONFIGURATION

// Pulls in the three ints and creates DummyClasses from them, and throws in some extras for good measure.
CONFIGURATION(ProcessSomeDummyClassesConfig)

private:
	DummyClass int1Dummy;
	DummyClass int2Dummy;
	DummyClass int3Dummy;

protected:
	void postInit() {
		int1Dummy = DummyClass(int1);
		int2Dummy = DummyClass(int2);
		int3Dummy = DummyClass(int3);
	}

	BEANS (
			(BEAN_INSTANCE, DummyClass&, int1Dummy),
			(BEAN_INSTANCE, DummyClass&, int2Dummy),
			(BEAN_INSTANCE, DummyClass&, int3Dummy),
			(BEAN, DummyClass*, "dummySingleton"),
			(BEAN, DummyClass*, corm::FactoryBeanCreator<DummyClass*>, "dummyFactory")
	)

	RESOURCES(
			(int&, int1),
			(int&, int2),
			(int&, int3)
	)

END_CONFIGURATION

// The "root" configuration which will pull in the others, and make sure that all beans were created properly.
CONFIGURATION(RootRuntimeConfig)

	DEPENDENCIES(ProcessSomeDummyClassesConfig, ProvideSomeMoreIntsConfig, ProvideSomeIntsConfig)

public:
	~RootRuntimeConfig() {
		delete(dummyFactory);
	}

protected:

	void postInit() {
		assert(123 == int1Dummy.getValue());
		assert(234 == int2Dummy.getValue());
		assert(456 == int3Dummy.getValue());
		assert(dummySingleton != NULL);
		assert(dummyFactory != NULL);
	}

	RESOURCES(
			(DummyClass&, int1Dummy),
			(DummyClass&, int2Dummy),
			(DummyClass&, int3Dummy),
			(DummyClass*, dummySingleton),
			(DummyClass*, dummyFactory)
	)

END_CONFIGURATION

#endif /* PERFORMANCE_RUNTIMECONFIG_H_ */
