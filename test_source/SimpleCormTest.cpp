#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include "SimpleCormConfig1.h"
#include "SimpleCormConfig2.h"
#include "SimpleCormAdder.h"

// Pull in the resources as globals (outside of class) for testing
RESOURCE(int, intBean1);
RESOURCE(int, intBean2);

BOOST_AUTO_TEST_SUITE(Simple_Corm_Test_Suite)

/*
 * Test case to ensure that the beans are properly initialized as expected
 */
BOOST_AUTO_TEST_CASE(Test_Beans) {
	// Check when pulled in using the "BEAN" macro
	BOOST_CHECK_EQUAL(123, intBean1);
	BOOST_CHECK_EQUAL(321, intBean2);
	// Check when pulled in using the function directly
	BOOST_CHECK_EQUAL("This is the strBean", provide_strBean());
	BOOST_CHECK_EQUAL(1.111, *provide_dblBean());
}

/*
 * Test case to ensure that a class can pull in the member resources as expected
 */
BOOST_AUTO_TEST_CASE(Test_Class_With_Member_Resources) {
	SimpleCormAdder adder;
	BOOST_CHECK_EQUAL(444, adder.add());
}

BOOST_AUTO_TEST_SUITE_END()
