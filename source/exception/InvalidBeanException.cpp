/*
 * InvalidBeanException.cpp
 *
 *  Created on: Jul. 25, 2020
 *      Author: Jaroslav Bosak
 */
#include "InvalidBeanException.h"

#include <sstream>

namespace corm {

/*
 * Helper functions which translates a vector to a human readable string form. Note, that the type of
 * stored within the vector must support the << operator.
 *
 * @template T the type of value stored within the vector
 *
 * @param v reference to the std::vector to convert to a string
 *
 * @return std::string the string representation of the vector
 */
template<typename T>
std::string vectorAsString(std::vector<T>& v) {
	std::stringstream ss("[");

	for (uint i = 0; i < v.size(); i++) {
		ss << v.at(i);

		if (i < (v.size() - 1))
			ss << ", ";
	}

	ss << "]";

	return ss.str();
}

/*
 * Constructor for the BeanDependencyCycleException that converts the cycle vector to a string
 */
BeanDependencyCycleException::BeanDependencyCycleException(std::vector<std::string>& cycle):
	std::runtime_error("Dependency cycle detected " + vectorAsString(cycle)) {
}

/*
 * Constructor for the ConfigurationMissingResources that converts the vector of missing resources to a string
 */
ConfigurationMissingResourcesException::ConfigurationMissingResourcesException(std::string configName, std::vector<std::string>& missing):
	std::runtime_error("Unable to create Configuration " + configName + " due to missing resources: " + vectorAsString(missing)){

}

}
