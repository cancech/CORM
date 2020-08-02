/*
 * Configuration.cpp
 *
 *  Created on: Aug. 2, 2020
 *      Author: Jaroslav Bosak
 */

#include "Configuration.h"

namespace corm {

// CTOR
BaseConfiguration::BaseConfiguration(BeanManager* manager): m_beanManager(manager) {}

/*
 * Get the default name
 */
std::string BaseConfiguration::getName() {
	return "Undefined";
}

/*
 * Get the default dependent configs
 */
std::vector<ConfigurationWrapperInterface*> BaseConfiguration::getDependentConfigurations(BeanManager* manager) {
	return std::vector<ConfigurationWrapperInterface*>();
}

/*
 * Get the default resource names
 */
std::vector<std::string> BaseConfiguration::getResourceNames() {
	return std::vector<std::string>();
}

/*
 * Get the default bean names
 */
std::vector<std::string> BaseConfiguration::getBeanNames() {
	return std::vector<std::string>();
}

/*
 * Initialize the configuration
 */
void BaseConfiguration::initialize() {
	postInit();
	provideBeans();
}

/*
 * Default post initialization
 */
void BaseConfiguration::postInit() {}

/*
 * Default bean provision
 */
void BaseConfiguration::provideBeans() {}

}
