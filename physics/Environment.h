#pragma once

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <list>
#include <vector>
#include "PhysicsObject.h"

struct Pair;

class Environment {
public:
	/// the horizontal size of the environment, measured in meters (m)
	float width;
	/// the vertical size of the environment, measured in meters (m)
	float height;
	/// the gravitational acceleration of the environment, measured downward in meters/second squared (m/s^2)
	float gravity;
	/// the constant amount of time that passes between each physics update
	float timestep;

	/// x-Axis for the environment
	Body xAxis;
	/// y-Axis for the environment
	Body yAxis;

	/// the list of bodies (objects) within the physics environment
	std::list <Body> bodyList;

	/// contains the possible collision pairs
	std::vector <Pair> pairs;


	Environment(float eWidth, float eHeight, float eGravity, float eTimestep);

	/**
	 * Adds a body to the environment to be rendered and included in physics calculations.
	 * Also initializes values such as the inverse mass
	 */
	void addBody(Body* body);

	/**
	 * Removes a body from the environment.
	 */
	void removeBody(Body* body);

	/**
	 * Broad phase, generate all possible non-duplicate pairs of bodies in the environment
	 */
	void generatePairs();

	/**
	 * Advances all objects in the environment 
	 */
	void step();
};

#endif