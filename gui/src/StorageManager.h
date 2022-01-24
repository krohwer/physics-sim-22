#pragma once

#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <vector>
#include <glm/glm.hpp>
#include "PhysicsObject.h"

class StorageManager {
private:
	/// stores the starting positions of objects
	std::vector<glm::vec3> startPositions;
	/// stores the starting speeds of objects
	std::vector<float> startSpeeds;
	/// stores the starting velocity direction of objects
	std::vector<float> startDirections;
public:
	/**
	 * clears all starting attributes from storage
	 */
	void clear() {
		startPositions.clear();
		startSpeeds.clear();
		startDirections.clear();
	}

	/**
	 * saves an object's starting attributes to storage
	 */
	void save(Body body) {
		startPositions.push_back(body.position);
		startSpeeds.push_back(body.vSpeed);
		startDirections.push_back(body.vDirection);
	}

	/**
	 * restores an object's attributes to those in storage
	 */
	void restore(Body& body, int bodyNumber) {
		body.position = startPositions[bodyNumber];
		body.vSpeed = startSpeeds[bodyNumber];
		body.vDirection = startDirections[bodyNumber];
	}
};

#endif