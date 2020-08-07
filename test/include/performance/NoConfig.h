#ifndef PERFORMANCE_NOCONFIG_H_
#define PERFORMANCE_NOCONFIG_H_

#include "DummyClass.h"
#include <cassert>

// Provider for int 1 and 2
class IntProvider1 {

public:
	int& getInt1() { return int1; }
	int& getInt2() { return int2; }

private:
	int int1 = 123;
	int int2 = 234;

};

// Provider for int 3
class IntProvider2 {

public:
	int& getInt3() { return int3; }

private:
	int int3 = 456;
};

// Class which provides DummyClass instances
class DummyClassProvider {

public:
	DummyClassProvider(): singleton(0) {
		int1Dummy = DummyClass(intProvider1.getInt1());
		int2Dummy = DummyClass(intProvider1.getInt2());
		int3Dummy = DummyClass(intProvider2.getInt3());
	}

	~DummyClassProvider() {
		if (singleton != 0)
			delete(singleton);
	}

	DummyClass& getIntDummy1() { return int1Dummy; }
	DummyClass& getIntDummy2() { return int2Dummy; }
	DummyClass& getIntDummy3() { return int3Dummy; }

	DummyClass* getSingletonDummy() {
		if (singleton == 0)
			singleton = new DummyClass();

		return singleton;
	}

	DummyClass* getFactoryDummy() {
		return new DummyClass();
	}

private:
	IntProvider1 intProvider1;
	IntProvider2 intProvider2;
	DummyClass int1Dummy;
	DummyClass int2Dummy;
	DummyClass int3Dummy;
	DummyClass* singleton;
};

// Class which brings everything in together and verifies the sanity of the objects
class CoreClass {
public:
	CoreClass() {
		dummyFactory = provider.getFactoryDummy();
	}

	~CoreClass() {
		delete(dummyFactory);
	}

	void verifyInstance() {
		assert(123 == provider.getIntDummy1().getValue());
		assert(234 == provider.getIntDummy2().getValue());
		assert(456 == provider.getIntDummy3().getValue());
		assert(provider.getSingletonDummy() != 0);
		assert(dummyFactory != 0);
	}

private:
	DummyClassProvider provider;
	DummyClass* dummyFactory;
};


#endif /* PERFORMANCE_NOCONFIG_H_ */
