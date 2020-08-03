#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include "Context.h"
#include <chrono>
#include <iostream>

#include "performance/RuntimeConfig.h"
#include "performance/SimpleConfig.h"
#include "performance/NoConfig.h"

int numOfReps =  100000;

// Helper to get the current time stamp in milliseconds
long getCurrentTimeMillis() {
	return std::chrono::duration_cast< std::chrono::milliseconds >(
			std::chrono::system_clock::now().time_since_epoch()).count();
}

// Helper to print the results of the test
void printResult(std::string testName, long start, long end) {
	double delta = (end - start) / 1000.0;
	std::cerr << testName << " completed in " << delta << " s" << std::endl;

}


// These tests are purely for clocking performance, therefore no need to run these at all times
BOOST_AUTO_TEST_SUITE(Performance_Test_Suite, * boost::unit_test::disabled())

BOOST_AUTO_TEST_CASE(Test_Runtime_Corm) {
	long start = getCurrentTimeMillis();
	for (int i = 0; i < numOfReps; i++) {
		corm::Context context;
		context.registerConfiguration<RootRuntimeConfig>();
		context.assemble();
	}
	long end = getCurrentTimeMillis();
	printResult("Runtime CORM", start, end);
}

BOOST_AUTO_TEST_CASE(Test_Simple_Corm) {
	long start = getCurrentTimeMillis();
	for (int i = 0; i < numOfReps; i++) {
		assembleAllBeans();
		delete(singletonInstance);
		singletonInstance = 0;
	}
	long end = getCurrentTimeMillis();
	printResult("Simple CORM", start, end);
}

BOOST_AUTO_TEST_CASE(Test_No_Config) {
	long start = getCurrentTimeMillis();
	for (int i = 0; i < numOfReps; i++) {
		CoreClass c;
		c.verifyInstance();
	}
	long end = getCurrentTimeMillis();
	printResult("No Config", start, end);

}

BOOST_AUTO_TEST_SUITE_END()
