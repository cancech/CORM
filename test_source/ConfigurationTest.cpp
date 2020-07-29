
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


BOOST_AUTO_TEST_SUITE(Configuration_Test_Suite)


BOOST_AUTO_TEST_CASE(Configuration_No_Resources_Required) {
	corm::BeanManager manager;
	corm::ConfigurationWrapper<NoResourceTestConfig> wrapper(&manager);
	wrapper.registerResources();
	BOOST_CHECK(wrapper.areResourcesSatisfied());

	NoResourceTestConfig* config = (NoResourceTestConfig*) wrapper.buildConfig();
	BOOST_CHECK_EQUAL(0, config->numTimesPostConstructCalled);
	BOOST_CHECK_EQUAL(0, config->numTimesProvideBeansCalled);
	config->initialize();
	BOOST_CHECK_EQUAL(1, config->numTimesPostConstructCalled);
	BOOST_CHECK_EQUAL(1, config->numTimesProvideBeansCalled);
	delete(config);
}

BOOST_AUTO_TEST_CASE(Configuration_Without_Required_Resources) {
	corm::BeanManager manager;
	corm::ConfigurationWrapper<MissingResourcesTestConfig> wrapper(&manager);
	wrapper.registerResources();
	BOOST_CHECK(!wrapper.areResourcesSatisfied());
	BOOST_REQUIRE_THROW(wrapper.buildConfig(), corm::ConfigurationMissingResourcesException);
}

BOOST_AUTO_TEST_CASE(Provider_and_Consumer) {
	corm::BeanManager manager;
	// The Consumer config cannot be initialized
	corm::ConfigurationWrapper<ConsumerTestConfig> consumerWrapper(&manager);
	consumerWrapper.registerResources();
	BOOST_CHECK(!consumerWrapper.areResourcesSatisfied());

	// The provider can be initialized
	corm::ConfigurationWrapper<ProviderTestConfig> providerWrapper(&manager);
	providerWrapper.registerResources();
	BOOST_CHECK(providerWrapper.areResourcesSatisfied());
	ProviderTestConfig* provider = (ProviderTestConfig*) providerWrapper.buildConfig();
	provider->initialize();

	// Now can initialize the consumer
	BOOST_CHECK(consumerWrapper.areResourcesSatisfied());
	ConsumerTestConfig* consumer = (ConsumerTestConfig*) consumerWrapper.buildConfig();
	consumer->initialize();

	// Verify that the values are correct
	BOOST_CHECK_EQUAL(provider->getSomeIntValuePtr(), consumer->getProviderSomeIntValue());
	BOOST_CHECK_EQUAL(manager.getBean<DummyClass*>("providerDummyClassSingleton"), consumer->getProviderDummyClassSingleton());

	delete(provider);
	delete(consumer);
}

BOOST_AUTO_TEST_SUITE_END()