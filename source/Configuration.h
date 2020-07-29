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
 * Base configuration which is to be extended for the purpose of creating a new configuration.
 */
class BaseConfiguration {

public:
	// CTOR
	BaseConfiguration(BeanManager* manager): beanManager(manager) {}
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
	// The bean manager that holds the beans for this context
	BeanManager* beanManager;

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

/*
 * Interface through which the ConfigurationManager can store and access the ConfigurationWrapper created
 * for each user defined configuration.
 */
class ConfigurationWrapperInterface {

public:
	// DTOR
	virtual ~ConfigurationWrapperInterface() = default;

	/*
	 * Get the name of the configuration associated with the wrapper.
	 *
	 * @return std::string the name of the configuration
	 */
	virtual std::string getName() const = 0;

	/*
	 * Check whether or not all required resources are available.
	 *
	 * @return true if there are no more resources the configuration is waiting on
	 */
	virtual bool areResourcesSatisfied() = 0;

	/*
	 * Build the configuration
	 *
	 * @return pointer to the configuration that this class is wrapped around
	 *
	 * @throw ConfigurationMissingResourcesException if the configuration is created when resources are still missing
	 */
	virtual BaseConfiguration* buildConfig() = 0;

	/*
	 * Get the vector of resources that are remaining to be fulfilled.
	 *
	 * @return std::vector of resource names that are not yet available
	 */
	virtual std::vector<std::string> getWaitingResources() const = 0;
};

/*
 * Helper class which wraps around a Configuration class to facilitate managing the availability of
 * the resources for the wrapper. It tracks which resources the configuration is still waiting to be
 * fulfilled and will then allow creating the Configuration class.
 */
template<class Config>
class ConfigurationWrapper: public ConfigurationWrapperInterface {

public:

	// CTOR
	ConfigurationWrapper(BeanManager* manager): beanManager(manager) {}

	/*
	 * The names of the resources that the Configuration requires are to be registered via registerResource
	 * in this method. Must be defined for each Configuration class type.
	 */
	virtual void registerResources();

	/*
	 * Get the name of the configuration that is wrapped.
	 */
	virtual std::string getName() const {
		return configName;
	}

	/*
	 * Check whether or not all required resources are available.
	 *
	 * @return true if there are no more resources the configuration is waiting on
	 */
	virtual bool areResourcesSatisfied() {
		waitingResources.erase(std::remove_if(waitingResources.begin(), waitingResources.end(),
				[this](std::string s) { return beanManager->containsBean(s); }), waitingResources.end());
		return waitingResources.empty();
	}

	/*
	 * Build the configuration
	 *
	 * @return pointer to the configuration that this class is wrapped around
	 *
	 * @throw ConfigurationMissingResourcesException if the configuration is created when resources are still missing
	 */
	virtual BaseConfiguration* buildConfig() {
		if (!areResourcesSatisfied())
			throw ConfigurationMissingResourcesException(configName, waitingResources);
		return new Config(beanManager);
	}

	/*
	 * Get the vector of waiting resources.
	 */
	virtual std::vector<std::string> getWaitingResources() const {
		return waitingResources;
	}

private:
	// Vector of the names of resources that the configuration is still waiting on
	std::vector<std::string> waitingResources;
	// Name of the configuration
	std::string configName = typeid(Config).name();
	// The bean manager that holds the beans for this context
	BeanManager* beanManager;

	/*
	 * Helper method to register a resource name
	 *
	 * @param std::string the name of the resource to register
	 */
	void registerResource(std::string name) {
		waitingResources.push_back(name);
	}
};

}

#endif /* CONFIGURATION_H_ */
