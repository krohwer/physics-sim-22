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

// 	void Solve();                 // Generate contact information
// 	void Initialize();            // Precalculations for impulse solving
// 	void ApplyImpulse();          // Solve impulse and apply
 	void PositionalCorrection();  // Naive correction of positional penetration
// 	void InfiniteMassCorrection();

	Body* A;
	Body* B;

	float penetration = 0.0f;     // Depth of penetration from collision
	glm::vec3 normal;      // From A to B
	//Vec2 contacts[2];     // Points of contact during collision
	int contact_count = 0; // Number of contacts that occured during collision
	float epsilon;         // Mixed restitution
	float df;              // Mixed dynamic friction
	float sf;              // Mixed static friction
};

#endif