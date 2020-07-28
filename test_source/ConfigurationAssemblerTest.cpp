
/*
 * ConfigurationManagerTest.cpp
 *
 *  Created on: Jul. 27, 2020
 *      Author: Jaroslav Bosak
 */
#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include "ConfigurationAssembler.h"

#include "config/SingleConfigMissingResourcesTestConfig.h"
#include "config/ProviderManagerTestConfig.h"
#include "config/ProviderConsumerManagerTestConfig.h"
#include "config/ConsumerManagerTestConfig.h"

BOOST_AUTO_TEST_SUITE(ConfigurationManager_Test_Suite)

BOOST_AUTO_TEST_CASE(Single_Configuration_Missing_Resources) {
	corm::ConfigurationAssembler manager;
	manager.registerConfiguration<SingleConfigMissingResourcesTestConfig>();
	BOOST_REQUIRE_THROW(manager.assemble(), corm::ConfigurationInitializationException);
}

BOOST_AUTO_TEST_CASE(Multiple_Configs) {
	corm::ConfigurationAssembler manager;
	manager.registerConfiguration<ConsumerManagerTestConfig>();
	manager.registerConfiguration<ProviderConsumerManagerTestConfig>();
	manager.registerConfiguration<ProviderManagerTestConfig>();
	manager.assemble();
}

BOOST_AUTO_TEST_CASE(No_Config_Registered) {
	corm::ConfigurationAssembler manager;
	manager.assemble();
}

BOOST_AUTO_TEST_SUITE_END()
