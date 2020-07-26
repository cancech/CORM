/*
 * Config1.h
 *
 *  Created on: Jul. 25, 2020
 *      Author: Jaroslav Bosak
 */

#ifndef CONFIG_CONFIG1_H_
#define CONFIG_CONFIG1_H_

#include "Configuration.h"
#include "DummyClass.h"

CONFIGURATION(Config1)

RESOURCES(Config1,
		(int*, an_Integer_Value),
		(std::string&, aStringBean),
		(DummyClass*, aDummyClassInstance),
		(DummyClass*, someOtherInstance)
)

#endif /* CONFIG_CONFIG1_H_ */
