#pragma once

#ifndef MANIFOLD_H
#define MANIFOLD_H

#include <glm/glm.hpp>

struct Body;

struct Manifold
{
	Manifold(Body* a, Body* b)
		: A(a)
		, B(b)
	{
	}

	void Solve(void);                 // Generate contact information
	void Initialize(void);            // Precalculations for impulse solving
	void ApplyImpulse(void);          // Solve impulse and apply
	void PositionalCorrection(void);  // Naive correction of positional penetration
	void InfiniteMassCorrection(void);

	Body* A;
	Body* B;

	float penetration;     // Depth of penetration from collision
	glm::vec3 normal;      // From A to B
	//Vec2 contacts[2];     // Points of contact during collision
	int contact_count; // Number of contacts that occured during collision
	float epsilon;         // Mixed restitution
	float df;              // Mixed dynamic friction
	float sf;              // Mixed static friction
};

#endif