
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

#include "Context.h"

#include "config/ContextTestConfigs.h"
#include "config/ContextTestBeanMismatchConfigs.h"
#include "config/ContextTestDependenctConfigs.h"

BOOST_AUTO_TEST_SUITE(ConfigurationManager_Test_Suite)

BOOST_AUTO_TEST_CASE(Single_Configuration_Missing_Resources) {
	corm::Context context;
	context.registerConfiguration<SingleConfigMissingResourcesTestConfig>();
	BOOST_REQUIRE_THROW(context.assemble(), corm::ConfigurationInitializationException);
}

BOOST_AUTO_TEST_CASE(Multiple_Configs) {
	corm::Context context;
	context.registerConfiguration<ConsumerManagerTestConfig, ProviderConsumerManagerTestConfig, ProviderManagerTestConfig>();
	context.assemble();
}

BOOST_AUTO_TEST_CASE(No_Config_Registered) {
	corm::Context context;
	context.assemble();
}

BOOST_AUTO_TEST_CASE(Config_Dependency) {
	corm::Context context;
	context.registerConfiguration<ConfigWithDeps>();
	context.assemble();
}

BOOST_AUTO_TEST_CASE(Wrong_Bean_Type) {
	corm::Context context;
	context.registerConfiguration<DummyBeanProviderTestConfig>();
	context.registerConfiguration<DummyBeanAsIntConsumerTestConfig>();
	BOOST_REQUIRE_THROW(context.assemble(), corm::InvalidBeanTypeException);
}

BOOST_AUTO_TEST_CASE(Wrong_Bean_Ptr) {
	corm::Context context;
	context.registerConfiguration<DummyBeanProviderTestConfig, PtrAsScalarConsumerTestConfig>();
	BOOST_REQUIRE_THROW(context.assemble(), corm::InvalidBeanTypeException);
}

BOOST_AUTO_TEST_SUITE_END()
