/*
 * BeanManagerTest.cpp
 *
 *  Created on: Jul. 12, 2020
 *      Author: Jaroslav Bosak
 */
#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>
#include <boost/any.hpp>

#include "BeanManager.h"
#include "DummyClass.h"

struct BeanManagerTestCreator {
	DummyClass create() {
		return DummyClass(135);
	}
};

BOOST_AUTO_TEST_SUITE(BeanManager_Test_Suite)

BOOST_AUTO_TEST_CASE(Register_bean_with_no_name) {
	try {
		corm::registerBean<int>("");
		BOOST_FAIL("Should not have been able to make it this far!");
	} catch (corm::InvalidBeanNameException& e) {
		// PASS: expect the second registration to trigger an exception
	}
}

BOOST_AUTO_TEST_CASE(Register_multiples_of_same_bean) {
	corm::registerBean<DummyClass, BeanManagerTestCreator>("multiple_of_same_bean");
	// Verify that the original bean was actually added
	DummyClass bean = corm::getBean<DummyClass>("multiple_of_same_bean");
	BOOST_CHECK_EQUAL(135, bean.getValue());

	try {
		corm::registerBean<int>("multiple_of_same_bean");
		BOOST_FAIL("Should not have been able to make it this far!");
	} catch (corm::InvalidBeanNameException& e) {
		// PASS: expect the second registration to trigger an exception
	}

	// Verify that the original bean is still there
	DummyClass bean2 = corm::getBean<DummyClass>("multiple_of_same_bean");
	BOOST_CHECK_EQUAL(135, bean2.getValue());
}

BOOST_AUTO_TEST_CASE(Retreive_bean_as_wrong_type) {
	corm::registerBean<DummyClass>("test_for_type_check");
	// Verify that the original bean was actually added
	DummyClass bean = corm::getBean<DummyClass>("test_for_type_check");
	BOOST_CHECK_EQUAL(0, bean.getValue());

	try {
		corm::getBean<int>("test_for_type_check");
		BOOST_FAIL("TYPE CONVERSION SHOULD HAVE FAILED BY NOW");
	} catch (corm::InvalidBeanTypeException& e) {
		// Expect the bean retrieval to fail
	}
}

BOOST_AUTO_TEST_CASE(Retreive_bean_not_yet_registered) {
	try {
		corm::getBean<boost::any>("this_bean_does_not_exist");
		BOOST_FAIL("SHOULD NOT BE ABLE TO RETRIEVE A BEAN");
	} catch (corm::InvalidBeanNameException& e) {
		// Expect the bean retrieval to fail
	}
}

BOOST_AUTO_TEST_CASE(Auto_Register_Creates_The_Bean) {
	// Make sure that the bean does not exist
	try {
		corm::getBean<boost::any>("auto_registered_bean");
		BOOST_FAIL("SHOULD NOT BE ABLE TO RETRIEVE A BEAN");
	} catch (corm::InvalidBeanNameException& e) {
		// Expect the bean retrieval to fail
	}

	// Enable auto registration
	corm::setAutoRegister(true);
	DummyClass* autoBean = corm::getBean<DummyClass*>("auto_registered_bean");
	BOOST_CHECK_EQUAL(0, autoBean->getValue());
	// Update the value of the bean
	autoBean->setValue(13579);
	BOOST_CHECK_EQUAL(13579, autoBean->getValue());
	// Make sure that this is a singleton bean
	auto otherPtr = corm::getBean<DummyClass*>("auto_registered_bean");
	BOOST_CHECK_EQUAL(13579, otherPtr->getValue());
	BOOST_CHECK_EQUAL(autoBean, otherPtr);

	// Testing complete, disable auto registration to prevent interfering with other tests
	corm::setAutoRegister(false);
	// Make sure that an invalid bean does not auto register anymore
	try {
		corm::getBean<boost::any>("some_other_bean_that_doesn't_exist_yet");
		BOOST_FAIL("SHOULD NOT BE ABLE TO RETRIEVE A BEAN");
	} catch (corm::InvalidBeanNameException& e) {
		// Expect the bean retrieval to fail
	}
}

BOOST_AUTO_TEST_SUITE_END()
