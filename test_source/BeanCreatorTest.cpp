/*
 * BeanCreatorTest.cpp
 *
 *  Created on: Jul. 11, 2020
 *      Author: Jaroslav Bosak
 */

#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include "BeanCreator.h"

#include "DummyClass.h"

BOOST_AUTO_TEST_SUITE(BeanCreator_Test_Suite)


BOOST_AUTO_TEST_CASE(Singleton_Scalar_Creator) {
	corm::SingletonBeanCreator<DummyClass> creator;

	DummyClass& dummy1 = creator.create();
	BOOST_CHECK_EQUAL(0, dummy1.getValue());

	DummyClass& dummy2 = creator.create();
	BOOST_CHECK_EQUAL(0, dummy2.getValue());

	// Verify that it is the same instance
	dummy1.setValue(5);
	BOOST_CHECK_EQUAL(5, dummy1.getValue());
	BOOST_CHECK_EQUAL(5, dummy2.getValue());

}


BOOST_AUTO_TEST_CASE(Singleton_Pointer_Creator) {
	corm::SingletonBeanCreator<DummyClass*> creator;

	DummyClass* dummy1 = creator.create();
	BOOST_CHECK_EQUAL(0, dummy1->getValue());

	DummyClass* dummy2 = creator.create();
	BOOST_CHECK_EQUAL(0, dummy2->getValue());

	// Verify that it is the same instance
	BOOST_CHECK_EQUAL(dummy1, dummy2);
	dummy1->setValue(5);
	BOOST_CHECK_EQUAL(5, dummy1->getValue());
	BOOST_CHECK_EQUAL(5, dummy2->getValue());
}


BOOST_AUTO_TEST_CASE(Factory_Scalar_Creator) {
	corm::FactoryBeanCreator<DummyClass> creator;

	DummyClass dummy1 = creator.create();
	BOOST_CHECK_EQUAL(0, dummy1.getValue());

	DummyClass dummy2 = creator.create();
	BOOST_CHECK_EQUAL(0, dummy2.getValue());

	// Verify that it is the same instance
	dummy1.setValue(5);
	BOOST_CHECK_EQUAL(5, dummy1.getValue());
	BOOST_CHECK_EQUAL(0, dummy2.getValue());
}


BOOST_AUTO_TEST_CASE(Factory_Pointer_Creator) {
	corm::FactoryBeanCreator<DummyClass*> creator;

	DummyClass* dummy1 = creator.create();
	BOOST_CHECK_EQUAL(0, dummy1->getValue());

	DummyClass* dummy2 = creator.create();
	BOOST_CHECK_EQUAL(0, dummy2->getValue());

	// Verify that it is the same instance
	BOOST_CHECK(dummy1 != dummy2);
	dummy1->setValue(5);
	BOOST_CHECK_EQUAL(5, dummy1->getValue());
	BOOST_CHECK_EQUAL(0, dummy2->getValue());
}

BOOST_AUTO_TEST_SUITE_END()
