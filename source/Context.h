/*
 * ConfigurationManager.h
 *
 *  Created on: Jul. 27, 2020
 *      Author: Jaroslav Bosak
 */

#ifndef CONTEXT_H_
#define CONTEXT_H_

#include <vector>
#include "Configuration.h"

namespace corm {

/*
 * Assembler for configurations, that will allow any number of configurations to be registered, and allow
 * them all to be assembled.
 */
class Context {

public:
	// DTOR
	virtual ~Context() {
		// Cleanup the configs which are still waiting
		for (ConfigurationWrapperInterface* i: waitingConfigs)
			delete(i);

		// Clean up the configs which have already been processed
		for (BaseConfiguration* c: activeConfigs)
			delete(c);
	}

	/*
	 * Register a single configuration
	 *
	 * @template the configuration class that is to be registered. It *must* extend corm::BaseConfiguration,
	 * 			 recommendation is to use the provided macros.
	 */
	template<class Config>
	void registerConfiguration() {
		ConfigurationWrapper<Config>* c = new ConfigurationWrapper<Config>(&beanManager);
		waitingConfigs.push_back(c);

		// Register any dependencies
		for(ConfigurationWrapperInterface* i: Config::getDependentConfigurations(&beanManager)) {
			waitingConfigs.push_back(i);
		}
	}

	/*
	 * Register any number of configurations (minimum 1)
	 *
	 * @template list all configuration classes that are to be registered. Each *must* extend corm::BaseConfiguration,
	 * 			 recommendation is to use the provided macros.
	 */
	template<class Config, class Config2, class... moreConfigs>
	void registerConfiguration() {
		// Register the first
		registerConfiguration<Config>();
		// Iterate through the remaining ones
		registerConfiguration<Config2, moreConfigs...>();
	}

	/*
	 * Attempt to assemble all of the registered configurations.
	 *
	 * @throws ConfigurationInitializationException if the dependencies for all configurations cannot be fulfilled
	 */
	void assemble() {
		// So long as configs are being processed, keep going. Extremely simplistic mechanism for
		// processing the dependencies across multiple configs.
		bool configProcessed = false;
		do {
			configProcessed = false;

			// Iterate through all registered configs and try to build and initialize them
			std::vector<ConfigurationWrapperInterface*>::iterator it = waitingConfigs.begin();
			while (it != waitingConfigs.end()) {
				ConfigurationWrapperInterface* wrapper = *it;

				if (wrapper->areResourcesSatisfied()) {
					// The config has all of its resources/dependencies satisfied
					configProcessed = true;
					// Create the config and initialize, process it
					BaseConfiguration* config = wrapper->buildConfig();
					try {
						config->initialize();
						activeConfigs.push_back(config);
					} catch (...) {
						// Prevent leaking memory if an exception is thrown during initialization
						delete(config);
						throw;
					}

					// Clean up and remove the temp classes
					it = waitingConfigs.erase(it);
					delete(wrapper);

				} else
					++it;

			}
		} while(configProcessed);

		if (!waitingConfigs.empty())
			throw ConfigurationInitializationException(waitingConfigs);
	}

private:
	// vector of configurations which are still waiting to be processed
	std::vector<ConfigurationWrapperInterface*> waitingConfigs;
	// vector of configurations which are processed and active
	std::vector<BaseConfiguration*> activeConfigs;
	// The bean manager that holds the beans for this context
	BeanManager beanManager;
};

}

#endif /* CONTEXT_H_ */
