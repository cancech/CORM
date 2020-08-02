/*
 * Context.cpp
 *
 *  Created on: Aug. 2, 2020
 *      Author: Jaroslav Bosak
 */

#include "Context.h"

namespace corm {

// DTOR
Context::~Context() {
	// Cleanup the configs which are still waiting
	for (ConfigurationWrapperInterface* i: waitingConfigs)
		delete(i);

	// Clean up the configs which have already been processed
	for (BaseConfiguration* c: activeConfigs)
		delete(c);
}

/*
 * Assemble the context
 */
void Context::assemble() {
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

	if (!waitingConfigs.empty()) {
		// Check to see if there is a circular dependency
		CircularDependencyChecker checker;
		for (ConfigurationWrapperInterface* i: waitingConfigs)
			checker.add(i->getName(), i->getWaitingResources(), i->getBeanNames());
		if (checker.checkForCycle()) {
			throw ConfigurationCycleException(checker.getCycle());
		}

		// If not, just throw the exception that cannot initialize
		throw ConfigurationInitializationException(waitingConfigs);
	}
}

}
