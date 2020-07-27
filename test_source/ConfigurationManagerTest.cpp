
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
#include "config/MissingResourcesTestConfig.h"
#include "config/NoResourceTestConfig.h"
#include "config/ProviderTestConfig.h"
#include "config/ConsumerTestConfig.h"


BOOST_AUTO_TEST_SUITE(ConfigurationManager_Test_Suite)


BOOST_AUTO_TEST_CASE(Configuration_No_Resources_Required) {
	corm::ConfigurationWrapper<NoResourceTestConfig> wrapper;
	wrapper.registerResources();
	BOOST_CHECK(wrapper.areResourcesSatisfied());

	NoResourceTestConfig* config = wrapper.buildConfig();
	BOOST_CHECK_EQUAL(0, config->numTimesPostConstructCalled);
	BOOST_CHECK_EQUAL(0, config->numTimesProvideBeansCalled);
	config->initialize();
	BOOST_CHECK_EQUAL(1, config->numTimesPostConstructCalled);
	BOOST_CHECK_EQUAL(1, config->numTimesProvideBeansCalled);
	delete(config);
}

BOOST_AUTO_TEST_CASE(Configuration_Without_Required_Resources) {
	corm::ConfigurationWrapper<MissingResourcesTestConfig> wrapper;
	wrapper.registerResources();
	BOOST_CHECK(!wrapper.areResourcesSatisfied());
	BOOST_REQUIRE_THROW(wrapper.buildConfig(), corm::ConfigurationMissingResourcesException);
}

BOOST_AUTO_TEST_CASE(Provider_and_Consumer) {
	// The Consumer config cannot be initialized
	corm::ConfigurationWrapper<ConsumerTestConfig> consumerWrapper;
	consumerWrapper.registerResources();
	BOOST_CHECK(!consumerWrapper.areResourcesSatisfied());

	// The provider can be initialized
	corm::ConfigurationWrapper<ProviderTestConfig> providerWrapper;
	providerWrapper.registerResources();
	BOOST_CHECK(providerWrapper.areResourcesSatisfied());
	ProviderTestConfig* provider = providerWrapper.buildConfig();
	provider->initialize();

	// Now can initialize the consumer
	BOOST_CHECK(consumerWrapper.areResourcesSatisfied());
	ConsumerTestConfig* consumer = consumerWrapper.buildConfig();
	consumer->initialize();

	// Verify that the values are correct
	BOOST_CHECK_EQUAL(provider->getSomeIntValuePtr(), consumer->getProviderSomeIntValue());
	BOOST_CHECK_EQUAL(corm::getBean<DummyClass*>("providerDummyClassSingleton"), consumer->getProviderDummyClassSingleton());
}

BOOST_AUTO_TEST_SUITE_END()
