/*
 * BeanProvider.h
 *
 *  Created on: Jul. 11, 2020
 *      Author: Jaroslav Bosak
 */

#ifndef BEANPROVIDER_H_
#define BEANPROVIDER_H_

#include "BeanCreator.h"
#include <type_traits>
#include <string>

namespace corm {

/*
 * The base base class for bean providers. Due to limitations of C++ this is the type that will be tracked
 * by the BeanManager, so as to allow it to not get bogged down with the template parameters.
 */
class BaseProvider {
public:
	// DTOR
	virtual ~BaseProvider() = default;

	/*
	 * Get the type of the bean as a string. Intended to be used purely for debugging purposes.
	 *
	 * @returns std::string the type of the bean as a string
	 */
	virtual std::string getType() = 0;
};

/*
 * Base class for the bean provider, that allows the BeanManager to determine what kind of bean is being provided,
 * without getting bogged down with the details of how exactly the bean is being created (i.e.: hide that template
 * parameter from the BeanManager).
 */
template<typename T>
class TypeProvider: public BaseProvider {

public:
	// DTOR
	virtual ~TypeProvider() = default;

	/*
	 * Get the type of the bean as a string. Intended to be used purely for debugging purposes.
	 *
	 * @returns std::string the type of the bean as a string
	 */
	virtual std::string getType() {
		return typeid(T).name();
	}

	/*
	 * Get the bean instance that this provider provides. Note that this class does not actually create
	 * the bean, and instead relies on a sub class to handle the bean creation.
	 */
	T getBean() {
		// The expectation is that the subclass will populate the bean instance field within the initBean method
		initBean();
		return bean;
	}

protected:
	// This is an interim storage vehicle where the bean created by the subclass is to be stored.
	T bean;

	/*
	 * Initialize the bean and store it within the bean instance field.
	 */
	virtual void initBean() = 0;
};

/*
 * Provides the bean that the Creator creates. This class primarily exists to make
 * the relationship between the Bean and Creator explicitly clear, and hide the details
 * of it from the BeanManager class.
 */
template<typename T, class Creator>
class BeanProvider: public TypeProvider<T> {

private:
	// The creator for determining how to create bean instances
	Creator creator;

	/*
	 * Initialize the bean, as per the creator, and store the resulting bean in the interim bean instance field container
	 */
	void initBean() {
		TypeProvider<T>::bean = creator.create();
	}
};

}

#endif /* BEANPROVIDER_H_ */
