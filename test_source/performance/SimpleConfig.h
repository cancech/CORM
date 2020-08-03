#ifndef PERFORMANCE_SIMPLECONFIG_H_
#define PERFORMANCE_SIMPLECONFIG_H_

// Avoid warnings due to macro clashes between the Simple and runtime CORM
#ifdef BEAN
#undef BEAN
#endif
#ifdef RESOURCE
#undef RESOURCE
#endif

#include "simple/SimpleCorm.h"
#include "DummyClass.h"
#include <cassert>

BEAN(int, int1) {
	return 123;
}

BEAN(int, int2) {
	return 234;
}

BEAN(int, int3) {
	return 456;
}

BEAN(DummyClass, dummyClass1) {
	return DummyClass(MTHD_NAME(int1));
}

BEAN(DummyClass, dummyClass2) {
	return DummyClass(MTHD_NAME(int2));
}

BEAN(DummyClass, dummyClass3) {
	return DummyClass(MTHD_NAME(int3));
}

DummyClass* singletonInstance = 0;
BEAN(DummyClass*, singletonDummy) {
	if (singletonInstance == 0)
		singletonInstance = new DummyClass();
	return singletonInstance;
}

BEAN(DummyClass*, factoryDummy) {
	return new DummyClass();
}

void assembleAllBeans() {
	assert(123 == MTHD_NAME(dummyClass1).getValue());
	assert(234 == MTHD_NAME(dummyClass2).getValue());
	assert(456 == MTHD_NAME(dummyClass3).getValue());
	assert(MTHD_NAME(singletonDummy) != 0);
	DummyClass* dummyFactory = MTHD_NAME(factoryDummy);
	assert(dummyFactory != 0);
	delete(dummyFactory);
}

#endif /* PERFORMANCE_SIMPLECONFIG_H_ */
