/*
 * SingleConfigMissingResourcesTestConfig.h
 *
 *  Created on: Jul. 27, 2020
 *      Author: Jaroslav Bosak
 */

#ifndef CONFIG_SINGLECONFIGMISSINGRESOURCESTESTCONFIG_H_
#define CONFIG_SINGLECONFIGMISSINGRESOURCESTESTCONFIG_H_

CONFIGURATION(SingleConfigMissingResourcesTestConfig)

RESOURCES(SingleConfigMissingResourcesTestConfig,
		(int, missingIntValue),
		(std::string&, missingStringReference)
)

#endif /* CONFIG_SINGLECONFIGMISSINGRESOURCESTESTCONFIG_H_ */
