#pragma once

#ifndef MANIFOLD_H
#define MANIFOLD_H

#include <glm/glm.hpp>

struct Body;

struct Manifold
{
	Manifold(Body* a, Body* b, float eGravity);

// 	void Solve();                 // Generate contact information
// 	void Initialize();            // Precalculations for impulse solving

	/**
	 * Resolve a collision between two physics bodies in a manifold
	 */
 	void ApplyImpulse();

	/**
	 * Forces overlapping objects to correct
	 */
	void PositionalCorrection();

	/**
	 * Corrects infinite mass objects so that they do not move
	 */
 	void InfiniteMassCorrection();

	float gravity;

	Body* A;
	Body* B;

	float penetration = 0.0f;     // Depth of penetration from collision
	glm::vec3 normal;      // From A to B
	glm::vec3 contacts[2];     // Points of contact during collision
	int contactCount = 0; // Number of contacts that occured during collision
	float epsilon = 0.2f;         // Mixed restitution
	float df = 0.2f;              // Mixed dynamic friction
	float sf = 0.3f;              // Mixed static friction
};

#endif