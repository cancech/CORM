/*
 * ConfigurationManager.h
 *
 *  Created on: Jul. 27, 2020
 *      Author: Jaroslav Bosak
 */

#ifndef CONFIGURATIONASSEMBLER_H_
#define CONFIGURATIONASSEMBLER_H_

#include <vector>
#include "Configuration.h"

namespace corm {

/*
 * Assembler for configurations, that will allow any number of configurations to be registered, and allow
 * them all to be assembled.
 */
class ConfigurationAssembler {

public:

	/*
	 * Register a single configuration
	 *
	 * @template the configuration class that is to be registered. This *must* extend corm::BaseConfiguration,
	 * 			 and it must have a no-arg constructor. Recommendation is to use the provided macros.
	 */
	template<class Config>
	void registerConfiguration() {
		ConfigurationWrapper<Config>* c = new ConfigurationWrapper<Config>();
		c->registerResources();
		waitingConfigs.push_back(c);
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
					config->initialize();

					// Clean up and remove the temp classes
					it = waitingConfigs.erase(it);
					delete(wrapper);
					delete(config);

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
};

}

#endif /* CONFIGURATIONASSEMBLER_H_ */
