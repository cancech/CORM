
/*
 * BeanProviderTest.cpp
 *
 *  Created on: Jul. 11, 2020
 *      Author: Jaroslav Bosak
 */
#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include "BeanProvider.h"
#include "DummyClass.h"

struct BeanProviderTestCreator {
	DummyClass create() {
		return DummyClass(789);
	}
};

BOOST_AUTO_TEST_SUITE(BeanProvider_Test_Suite)

BOOST_AUTO_TEST_CASE(Bean_creator_provider) {
	corm::BeanCreatorProvider<DummyClass, BeanProviderTestCreator> provider;
	DummyClass bean = provider.getBean();
	BOOST_CHECK_EQUAL(789, bean.getValue());
	// The result need not be exactly the class name, so only need to make sure that the class name is contained/represented
	BOOST_CHECK(provider.getType().find("DummyClass") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(Bean_instance_provider) {
	DummyClass instance(987);
	corm::BeanInstanceProvider<DummyClass*> provider(&instance);

	DummyClass* bean = provider.getBean();
	BOOST_CHECK_EQUAL(instance.getValue(), bean->getValue());
	BOOST_CHECK_EQUAL(&instance, bean);
}

BOOST_AUTO_TEST_SUITE_END()
