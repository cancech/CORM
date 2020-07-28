/*
 * InvalidBeanException.h
 *
 *  Created on: Jul. 12, 2020
 *      Author: Jaroslav Bosak
 */

#ifndef INVALIDBEANEXCEPTION_H_
#define INVALIDBEANEXCEPTION_H_

#include <exception>
#include <boost/stacktrace.hpp>
#include <boost/exception/all.hpp>
#include <vector>
#include <iostream>

typedef boost::error_info<struct tag_stacktrace, boost::stacktrace::stacktrace> traced;

namespace corm {

class ConfigurationWrapperInterface;

// TODO see if this does what I want it to do, in terms of throwing an exception with a stack trace
template<class E>
void throw_with_trace(const E &e) {
	throw boost::enable_error_info(e) << traced(boost::stacktrace::stacktrace());
}

/*
 * Exception which is to be thrown when attempting to use an invalid name for a bean. This includes:
 *      * registering a bean with a name that already exists
 *      * retrieving a bean that has not yet been registered
 */
struct InvalidBeanNameException: public std::runtime_error {
	InvalidBeanNameException(std::string name, std::string reason) :
			std::runtime_error("Invalid bean name \"" + name + "\": " + reason) {
	}
};

/*
 * Exception which is to be throw when attempting to retrieve a bean which is of a different type than
 * what is desired (i.e.: register bean is Foo, but trying to retrieve Bar).
 */
struct InvalidBeanTypeException: public std::runtime_error {
	InvalidBeanTypeException(std::string name, std::string wanted, std::string actual) :
			std::runtime_error("Invalid bean type for \"" + name + "\": wanted " + wanted + " but was " + actual) {
	}
};

/*
 * Exception which is thrown when attempting to get a bean which exists within a dependency cycle (i.e.: BeanA depends on
 * BeanB, which depends on BeanA).
 */
struct BeanDependencyCycleException: public std::runtime_error {
	BeanDependencyCycleException(std::vector<std::string>& cycle);
};

/*
 * Exception which is thrown when attempting to create a configuration while it is still waiting on resources.
 */
struct ConfigurationMissingResourcesException: public std::runtime_error {
	ConfigurationMissingResourcesException(std::string configName, std::vector<std::string>& missing);
};

/*
 * Exception which is thrown when an issue is encountered when attempting to initialize the configurations.
 */
struct ConfigurationInitializationException: public std::runtime_error {
	ConfigurationInitializationException(std::vector<ConfigurationWrapperInterface*>& wrappers);
};

}

#endif /* INVALIDBEANEXCEPTION_H_ */
