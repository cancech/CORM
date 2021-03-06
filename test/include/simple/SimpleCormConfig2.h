#ifndef SIMPLECORMCONFIG2_H_
#define SIMPLECORMCONFIG2_H_

#include "corm/simple/SimpleCorm.h"

/*
 * Provides intBean1 for testing
 */
BEAN(int, intBean1) {
	return 123;
}

/*
 * Provides dblBean for testing
 */
BEAN(double*, dblBean) {
	return new double(1.111);
}

namespace testing {
	BEAN(int, nsIntBean2) {
		return 100;
	}
}

#endif /* SIMPLECORMCONFIG2_H_ */
