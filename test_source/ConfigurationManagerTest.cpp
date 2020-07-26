/*
 * ConfigurationManagerTest.cpp
 *
 *  Created on: Jul. 25, 2020
 *      Author: Jaroslav Bosak
 */
#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include "DummyClass.h"
#include "config/Config1.h"


BOOST_AUTO_TEST_SUITE(ConfigurationManager_Test_Suite)

BOOST_AUTO_TEST_CASE(Configuration_Without_Required_Resources) {
	corm::ConfigurationWrapper<Config1> wrapper;
	wrapper.registerResources();
	BOOST_CHECK(!wrapper.areResourcesSatisfied());
	BOOST_REQUIRE_THROW(wrapper.buildConfig(), corm::ConfigurationMissingResourcesException);
}

BOOST_AUTO_TEST_SUITE_END()
