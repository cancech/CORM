#include "corm/BeanManager.h"

namespace corm {

// DTOR
BeanManager::~BeanManager() {
	for (std::pair<std::string, BaseProvider*> i: repo)
		delete(i.second);
}

/*
 * Check whether or not the bean already exists
 */
bool BeanManager::containsBean(std::string name) {
	return repo.count(name);
}

/*
 * Verify that the bean can be added, throwing an exception otherwise.
 */
void BeanManager::verifyCanAddBean(std::string name) {
	if (name.empty())
		throw InvalidBeanNameException(name, "bean name cannot be empty");
	else if (containsBean(name))
		throw InvalidBeanNameException(name, "bean is already registered");
}

}
