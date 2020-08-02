#include "CircularDependencyChecker.h"
#include <iostream>
#include <algorithm>

namespace corm {

/*
 * Add the configuration
 */
void CircularDependencyChecker::add(std::string name, std::vector<std::string> resources,
		std::vector<std::string> beans) {
	configResources[name] = resources;
	for(std::string s: beans)
		beanSource[s] = name;
}

/*
 * Check whether a cycle is present
 */
bool CircularDependencyChecker::checkForCycle() {
	// Iterate over each configuration
	for (std::pair<std::string,  std::vector<std::string>> p: configResources) {
		// Check each configuration to see whether or not it is part of a cycle.
		if (checkAllResources(p.first)) {
			minimizeCycle();
			return true;
		}
	}

	return false;
}

/*
 * Minimize the reported cycle the smallest it can be.
 */
void CircularDependencyChecker::minimizeCycle() {
	auto start = std::find(cycle.begin(), cycle.end(), cycle[cycle.size() - 1]);
	cycle.erase(cycle.begin(), start);
}

/*
 * Get the detected cycle.
 */
const std::vector<std::string>& CircularDependencyChecker::getCycle() {
	return cycle;
}

/*
 * Check all resources for the given configuration
 */
bool CircularDependencyChecker::checkAllResources(std::string configName) {
	// Sanity check, make sure that this configuration has been registered
	if (!configResources.count(configName))
		return false;

	// Check all of the resources that the configuration requires
	std::vector<std::string>& resources = configResources[configName];
	for (std::string res: resources) {
		// Check if a cycle was found
		if (followResource(configName, res))
			return true;
	}

	// No cycle was found!
	return false;
}

/*
 * Follow the resource required by the configuration, and see whether a cycle is present.
 */
bool CircularDependencyChecker::followResource(std::string configName, std::string resourceName) {
	// Build the display name and check if it has been seen.
	std::string compoundName = configName + "::" + resourceName;
	bool hasCycle = std::find(cycle.begin(), cycle.end(), compoundName) != cycle.end();
	cycle.push_back(compoundName);

	// Check if a cycle has been detected (the display name is already present in the vector
	if (hasCycle)
		return true;

	// Follow the source of the resource
	if (beanSource.count(resourceName)) {
		if (checkAllResources(beanSource[resourceName]))
			return true;
	}

	// No cycle detected
	cycle.pop_back();
	return false;
}

}
