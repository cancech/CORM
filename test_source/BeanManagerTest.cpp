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
	corm::ValueWrapper<DummyClass>* create() {
		return new corm::ValueWrapper<DummyClass>(135);
	}
};

// Classes to be used to test the cycle detection capabilities
class A_depends_on_B;
class B_depends_on_A;

class A_depends_on_B {
	B_depends_on_A* b = corm::getBean<B_depends_on_A*>("B_depends_on_A");
};

class B_depends_on_A {
	A_depends_on_B* a = corm::getBean<A_depends_on_B*>("A_depends_on_B");
};

BOOST_AUTO_TEST_SUITE(BeanManager_Test_Suite)

BOOST_AUTO_TEST_CASE(Register_bean_with_no_name) {
	BOOST_REQUIRE_THROW(corm::registerBean<int&>(""), corm::InvalidBeanNameException);
}

BOOST_AUTO_TEST_CASE(Register_singleton_reference) {
	corm::registerBean<DummyClass&>("singleton_reference");

	DummyClass& bean1 = corm::getBean<DummyClass&>("singleton_reference");
	DummyClass& bean2 = corm::getBean<DummyClass&>("singleton_reference");

	BOOST_CHECK_EQUAL(&bean1, &bean2);
}

BOOST_AUTO_TEST_CASE(Register_singleton_pointer) {
	corm::registerBean<DummyClass*>("singleton_pointer");

	DummyClass* bean1 = corm::getBean<DummyClass*>("singleton_pointer");
	DummyClass* bean2 = corm::getBean<DummyClass*>("singleton_pointer");

	BOOST_CHECK_EQUAL(bean1, bean2);
	BOOST_CHECK_EQUAL(bean1->getValue(), bean2->getValue());
}

BOOST_AUTO_TEST_CASE(Register_factory_scalar) {
	corm::registerBean<DummyClass, corm::FactoryBeanCreator<DummyClass>>("factory_scalar");

	DummyClass bean1 = corm::getBean<DummyClass>("factory_scalar");
	DummyClass bean2 = corm::getBean<DummyClass>("factory_scalar");

	BOOST_CHECK(&bean1 != &bean2);
}

BOOST_AUTO_TEST_CASE(Register_factory_pointer) {
	corm::registerBean<DummyClass*, corm::FactoryBeanCreator<DummyClass*>>("factory_pointer");

	DummyClass* bean1 = corm::getBean<DummyClass*>("factory_pointer");
	DummyClass* bean2 = corm::getBean<DummyClass*>("factory_pointer");

	BOOST_CHECK(bean1 != bean2);
	BOOST_CHECK(bean1 != NULL);
	BOOST_CHECK(bean2 != NULL);
}

BOOST_AUTO_TEST_CASE(Register_multiples_of_same_bean) {
	corm::registerBean<DummyClass, BeanManagerTestCreator>("multiple_of_same_bean");
	// Verify that the original bean was actually added
	DummyClass bean = corm::getBean<DummyClass>("multiple_of_same_bean");
	BOOST_CHECK_EQUAL(135, bean.getValue());

	BOOST_REQUIRE_THROW(corm::registerBean<int*>("multiple_of_same_bean"), corm::InvalidBeanNameException);

	// Verify that the original bean is still there
	DummyClass bean2 = corm::getBean<DummyClass>("multiple_of_same_bean");
	BOOST_CHECK_EQUAL(135, bean2.getValue());
}

BOOST_AUTO_TEST_CASE(Retreive_bean_as_wrong_type) {
	corm::registerBean<DummyClass&>("test_for_type_check");
	// Verify that the original bean was actually added
	DummyClass bean = corm::getBean<DummyClass&>("test_for_type_check");
	BOOST_CHECK_EQUAL(0, bean.getValue());

	BOOST_REQUIRE_THROW(corm::getBean<int>("test_for_type_check"), corm::InvalidBeanTypeException);
}

BOOST_AUTO_TEST_CASE(Retreive_bean_not_yet_registered) {
	BOOST_CHECK(!corm::BeanManager::instance()->containsBean("this_bean_does_not_exist"));

#ifdef ENABLE_BEAN_AUTOREGISTRATION
	// Enable auto registration
	DummyClass *autoBean = corm::getBean<DummyClass*>("this_bean_does_not_exist");
	BOOST_CHECK_EQUAL(0, autoBean->getValue());
	// Update the value of the bean
	autoBean->setValue(13579);
	BOOST_CHECK_EQUAL(13579, autoBean->getValue());
	// Make sure that this is a singleton bean
	auto otherPtr = corm::getBean<DummyClass*>("this_bean_does_not_exist");
	BOOST_CHECK_EQUAL(13579, otherPtr->getValue());
	BOOST_CHECK_EQUAL(autoBean, otherPtr);
#else
	BOOST_REQUIRE_THROW(corm::getBean<boost::any>("this_bean_does_not_exist"), corm::InvalidBeanNameException);
#endif
}

BOOST_AUTO_TEST_CASE(Bean_instance_scalar) {
	DummyClass instance(101);
	corm::registerBeanInstance<DummyClass>("instance_scalar", instance);

	DummyClass bean = corm::getBean<DummyClass>("instance_scalar");
	BOOST_CHECK_EQUAL(instance.getValue(), bean.getValue());
	BOOST_CHECK(&instance != &bean);
}

BOOST_AUTO_TEST_CASE(Bean_instance_reference) {
	DummyClass instance(101);
	corm::registerBeanInstance<DummyClass&>("instance_reference", instance);

	DummyClass& bean = corm::getBean<DummyClass&>("instance_reference");
	BOOST_CHECK_EQUAL(instance.getValue(), bean.getValue());
	BOOST_CHECK_EQUAL(&instance, &bean);
}

BOOST_AUTO_TEST_CASE(Bean_instance_pointer) {
	DummyClass* instance = new DummyClass(202);
	corm::registerBeanInstance<DummyClass*>("instance_pointer", instance);

	DummyClass* bean = corm::getBean<DummyClass*>("instance_pointer");
	BOOST_CHECK_EQUAL(instance->getValue(), bean->getValue());
	BOOST_CHECK_EQUAL(instance, bean);
}

BOOST_AUTO_TEST_CASE(Check_for_Cyclic_Bean_Dependencies) {
	// Register the two beans (will not get created until requested)
	corm::registerBean<A_depends_on_B*>("A_depends_on_B");
	corm::registerBean<B_depends_on_A*>("B_depends_on_A");

	BOOST_REQUIRE_THROW(corm::getBean<A_depends_on_B*>("A_depends_on_B"), corm::BeanDependencyCycleException);
	BOOST_REQUIRE_THROW(corm::getBean<B_depends_on_A*>("B_depends_on_A"), corm::BeanDependencyCycleException);
}

BOOST_AUTO_TEST_SUITE_END()
