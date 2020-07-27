/*
 * AbstractConfig.h
 *
 *  Created on: Jul. 25, 2020
 *      Author: Jaroslav Bosak
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <vector>

#include "ConfigurationMacro.h"
#include "BeanManager.h"

namespace corm {

/*
 * Helper class which wraps around a Configuration class to facilitate managing the availability of
 * the resources for the wrapper. It tracks which resources the configuration is still waiting to be
 * fulfilled and will then allow creating the Configuration class.
 */
template<class Config>
class ConfigurationWrapper {

public:
	/*
	 * The names of the resources that the Configuration requires are to be registered via registerResource
	 * in this method. Must be defined for each Configuration class type.
	 */
	void registerResources();

	/*
	 * Check whether or not all required resources are available.
	 *
	 * @return true if there are no more resources the configuration is waiting on
	 */
	bool areResourcesSatisfied() {
		waitingResources.erase(std::remove_if(waitingResources.begin(), waitingResources.end(),
				[](std::string s) { return BeanManager::instance()->containsBean(s); }), waitingResources.end());
		return waitingResources.empty();
	}

	/*
	 * Build the configuration
	 *
	 * @return pointer to the configuration that this class is wrapped around
	 *
	 * @throw ConfigurationMissingResourcesException if the configuration is created when resources are still missing
	 */
	Config* buildConfig() {
		if (!areResourcesSatisfied())
			throw ConfigurationMissingResourcesException(configName, waitingResources);
		return new Config();
	}

private:
	// Vector of the names of resources that the configuration is still waiting on
	std::vector<std::string> waitingResources;
	// Name of the configuration
	std::string configName = typeid(Config).name();

	/*
	 * Helper method to register a resource name
	 *
	 * @param std::string the name of the resource to register
	 */
	void registerResource(std::string name) {
		waitingResources.push_back(name);
	}
};

/*
 * Base configuration which is to be extended for the purpose of creating a new configuration.
 */
class BaseConfiguration {

public:
	// CTOR
	BaseConfiguration() = default;
	// DTOR
	virtual ~BaseConfiguration() = default;

	/*
	 * Initialize the configuration, triggers the postInit and provideBeans methods (in that
	 * order).
	 */
	void initialize() {
		postInit();
		provideBeans();
	}

protected:
	/*
	 * Called after the configuration has been created, to allow steps to be taken once all required
	 * resources become available.
	 */
	virtual void postInit() {}

	/*
	 * Called after postContruct completes, to allow the configuration to provide any/all beans.
	 */
	virtual void provideBeans() {}

};

}

#endif /* CONFIGURATION_H_ */
