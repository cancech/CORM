#ifndef CONTEXT_H_
#define CONTEXT_H_

#include <vector>
#include "Configuration.h"
#include "CircularDependencyChecker.h"

namespace corm {

/*
 * Assembler for configurations, that will allow any number of configurations to be registered, and allow
 * them all to be assembled.
 */
class Context {

public:
	// DTOR
	virtual ~Context();

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
	 * @throws InvalidBeanTypeException if there is a bean type mismatch
	 */
	void assemble();

private:
	// vector of configurations which are still waiting to be processed
	std::vector<ConfigurationWrapperInterface*> waitingConfigs;
	// vector of configurations which are processed and active
	std::vector<BaseConfiguration*> activeConfigs;
	// The bean manager that holds the beans for this context
	BeanManager beanManager;

	/*
	 * Try to load the configuration that belongs to the wrapper.
	 *
	 * @param wrapper ConfigurationWrapperInterface* pointer to the wrapper for the configuration to try to load
	 *
	 * @return true if the configuration was loaded successfully
	 */
	bool loadConfig(ConfigurationWrapperInterface* wrapper);

	/*
	 * Verify the context and make sure that everything is properly loaded.
	 *
	 * @throws ConfigurationInitializationException if the dependencies for all configurations cannot be fulfilled
	 */
	void verifyContext();
};

}

#endif /* CONTEXT_H_ */
