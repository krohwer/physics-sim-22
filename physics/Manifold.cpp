#include "Manifold.h"

#include <algorithm>
#include <glm/glm.hpp>
#include <iostream>

#include "PhysicsObject.h"
#include "PhysicsMath.h"

Manifold::Manifold(Body* a, Body* b, float eGravity) {
	A = a;
	B = b;
	gravity = eGravity;
	normal = glm::vec3(0.0f);
	contacts[0] = {};
}

void Manifold::ApplyImpulse() {

	// if both objects have infinite mass, basically just ignore the collision
	if (Equal((A->inverseMass + B->inverseMass), 0.0f)) {
		InfiniteMassCorrection();
		return; // I know I know, but I don't feel like wrapping the whole thing
	}

	glm::vec3 relativeVelocity = B->velocity - A->velocity;
	float veloctyAlongNormal = glm::dot(relativeVelocity, normal);

	// only continue if the objects are not separating
	if (veloctyAlongNormal < 0) {
		// calculate the restitution
		epsilon = std::min(A->restitution, B->restitution);
		// if an object's relative velocity is only due to gravity over the timestep, we want a resting collision with no restitution
		float gdt = gravity * timestep;
		if (lenSqr(relativeVelocity) < (gdt * gdt) + EPSILON) {
			epsilon = 0.0f;
		}
		else {
			// report an impact
			std::cout << "Collision" << std::endl;
		}

		// calculate inverse mass sum and impulse scalar j
		float inverseMassSum = A->inverseMass + B->inverseMass;
		float j = -(1 + epsilon) * veloctyAlongNormal;
		j /= inverseMassSum;

		// apply impulse
		// always subtract from a and add to b since the normal points from a to b
		glm::vec3 impulse = j * normal;
		A->velocity -= A->inverseMass * impulse;
		B->velocity += B->inverseMass * impulse;

		// friction impulse, after standard impulse

		relativeVelocity = B->velocity - A->velocity;

		// solve for the tangent vector
		glm::vec3 tangent = relativeVelocity - (glm::dot(relativeVelocity, normal) * normal);
		if (length(tangent) != 0.0f)
			tangent = glm::normalize(tangent);
		// TODO:  NORMALIZE DOESN'T WORK THANKS GLM

		// solve for the magnitude to apply along the friction vector
		float jtangent = (-glm::dot(relativeVelocity, tangent));
		jtangent /= inverseMassSum;

		// check to avoid applying tiny friction impulses
		if (!Equal(jtangent, 0.0f)) {
			// normally solve for the static friction coefficient here, but I'm not sure how I'll do that yet so hardcoded wood rn
			float staticMu = 0.4f;

			// clamp the magnitude of the friction and create an impulse vector
			glm::vec3 frictionImpluse(0.0f);
			if (glm::abs(jtangent) < j * staticMu) {
				frictionImpluse = jtangent * tangent;
			}
			else {
				float dynamicMu = 0.2f;
				frictionImpluse = -j * tangent * dynamicMu;
			}
			// apply friction impulse
			A->velocity -= A->inverseMass * frictionImpluse;
			B->velocity += B->inverseMass * frictionImpluse;
		}
	}
	// recalculate each velocity vector so the UI displays properly
	A->computeVelocityVector();
	B->computeVelocityVector();
	// positional correction
	PositionalCorrection();
}

void Manifold::PositionalCorrection() {
	const float percent = 1.0f; // usually 20% to 80%
	const float slop = 0.0001f; // usually 0.01 to 0.1
	glm::vec3 correction = (std::max(penetration - slop, 0.0f) / (A->inverseMass + B->inverseMass)) * percent * normal;
	A->position -= A->inverseMass * correction;
	B->position += B->inverseMass * correction;
}

void Manifold::InfiniteMassCorrection() {
	A->velocity = glm::vec3(0.0f);
	B->velocity = glm::vec3(0.0f);

	// recalculate each velocity vector so the UI displays properly
	A->computeVelocityVector();
	B->computeVelocityVector();
}
