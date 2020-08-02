#ifndef SIMPLECORMCONFIG1_H_
#define SIMPLECORMCONFIG1_H_

#include "simple/SimpleCorm.h"
#include <string>

/*
 * Provides intBean2 for testing
 */
BEAN(int, intBean2) {
	return 321;
}

/*
 * Provides strBean for testing
 */
BEAN(std::string, strBean) {
	return "This is the strBean";
}

namespace testing {
	BEAN(int, nsIntBean1) {
		return 1;
	}
}

#endif /* SIMPLECORMCONFIG1_H_ */
