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
#include <typeinfo>

typedef boost::error_info<struct tag_stacktrace, boost::stacktrace::stacktrace> traced;


namespace corm {

// TODO see if this does what I want it to do, in terms of throwing an exception with a stack trace
template <class E>
void throw_with_trace(const E& e) {
    throw boost::enable_error_info(e)
        << traced(boost::stacktrace::stacktrace());
}

/*
 * Exception which is to be thrown when attempting to use an invalid name for a bean. This includes:
 *      * registering a bean with a name that already exists
 *      * retrieving a bean that has not yet been registered
 */
struct InvalidBeanNameException : public std::runtime_error {
	InvalidBeanNameException(std::string name, std::string reason) :
		std::runtime_error("Invalid bean name \"" + name + "\": " + reason) {}
};

/*
 * Exception which is to be throw when attempting to retrieve a bean which is of a different type than
 * what is desired (i.e.: register bean is Foo, but trying to retrieve Bar).
 */
struct InvalidBeanTypeException : public std::runtime_error {
	InvalidBeanTypeException(std::string name, std::string wanted, std::string actual):
		std::runtime_error("Invalid bean type for \"" + name +"\": wanted " + wanted + " but was " + actual) {}
};

}

#endif /* INVALIDBEANEXCEPTION_H_ */
