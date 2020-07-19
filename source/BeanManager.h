/*
 * ObjectRepositoryManager.h
 *
 *  Created on: Jul. 11, 2020
 *      Author: Jaroslav Bosak
 */

#ifndef BEANMANAGER_H_
#define BEANMANAGER_H_

#include <string>
#include <map>

#include <boost/any.hpp>

#include "BeanProvider.h"
#include "exception/InvalidBeanException.h"

namespace corm {

/*
 * Manager responsible for tracking all bean providers, and retrieving the beans from when
 * on request. The bean must be first registered by a producer, in order for it to be retrieved.
 * Each bean is registered with a unique name, and only one bean can be registered using a given
 * name. Exceptions will be thrown if a duplicate registration is performed under the same name.
 *
 * An optional auto-register capability can be enabled, to allow registering beans when they are
 * requested. If this capability is enabled then:
 *        * All auto-registered beans will be singleton (the default creator for beans)
 *        * The bean which is to be "auto-registered" must have a default constructor (the bean will
 *        be created via its default constructor)
 */
class BeanManager {

public:
	/*
	 * Get the singleton instance of the bean manager.
	 */
	static BeanManager* instance() {
		static BeanManager *INSTANCE = new BeanManager();
		return INSTANCE;
	}

	/*
	 * Enable/disable the auto registration capability of the manager.
	 *
	 * @param value bool true if auto registration is to be enabled.
	 */
	void setAutoRegister(bool value) {
		autoRegister = value;
	}

	/*
	 * Register a bean with the manager. The bean will rely on the specified creator for
	 * bean management/creation.
	 *
	 * @template Type the type of the bean which is to be registered
	 * @template Creator the creator which is to be used to instantiate/manage the bean
	 *           defaults to SingletonBeanCreator
	 *
	 * @param name std::string the name of the bean to register
	 *
	 * @throws InvalidBeanNameException if a bean by that name already exists
	 */
	template<typename Type, class Creator = SingletonBeanCreator<Type>>
	void registerBean(std::string name) {
		if (name.empty())
			throw InvalidBeanNameException(name, "bean name cannot be empty");
		else if (containsBean(name))
			throw InvalidBeanNameException(name, "bean is already registered");

		repo[name] = new BeanProvider<Type, Creator>();
	}

	/*
	 * Get a bean from the manager that has been registered under the specified name. If auto
	 * registration is enabled, if the request bean does not exist it will automatically be
	 * registered. Note that any auto-registered beans will be singleton and must have a default
	 * constructor.
	 *
	 * @template Type the type of the bean that is desired
	 *
	 * @param name std::string the name of the bean.
	 *
	 * @returns Type the desired bean
	 *
	 * @throws InvalidBeanNameException if no bean of the specified name is registered (and auto registration is disabled)
	 * @throws InvalidBeanTypeException if the registered bean is of a different type than what is requested
	 */
	template<typename Type>
	Type getBean(std::string name) {
		if (!containsBean(name)) {
			if (autoRegister)
				registerBean<Type>(name);
			else
				throw InvalidBeanNameException(name, "no bean of that name available");
		}

		BaseProvider *baseProvider = repo[name];
		TypeProvider<Type> *typeProvider = dynamic_cast<TypeProvider<Type>*>(baseProvider);

		if (typeProvider)
			return typeProvider->getBean();

		throw InvalidBeanTypeException(name, typeid(Type).name(), baseProvider->getType());
	}

private:
	// Repository of all registered beans
	std::map<std::string, BaseProvider*> repo;
	// Flag for whether missing beans should automatically be registered
	bool autoRegister = false;

	// CTOR and DTOR are hidden due to singleton
	BeanManager() = default;
	~BeanManager() = default;

	/*
	 * Convenience method to check if a bean of the given name is already registered.
	 *
	 * @param name std::string the name of the bean to check for
	 *
	 * @returns bool true if the bean is already registered
	 */
	bool containsBean(std::string name) {
		return repo.count(name);
	}
};

/*
 * Convenience function which updates the auto registration of the BeanManager
 *
 * @param value bool true if auto registration is to be enabled.
 */
void setAutoRegister(bool value) {
	BeanManager::instance()->setAutoRegister(value);
}

/*
 * Convenience function for registering beans with the manager.
 */
template<typename T, class U = SingletonBeanCreator<T>>
void registerBean(std::string name) {
	BeanManager::instance()->registerBean<T, U>(name);
}

/*
 * Convenience function for retrieving beans from the manager.
 */
template<typename T>
T getBean(std::string name) {
	return BeanManager::instance()->getBean<T>(name);
}

}

#endif /* BEANMANAGER_H_ */
