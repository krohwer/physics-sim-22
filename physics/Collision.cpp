#include "Collision.h"

#include <algorithm>
#include <glm/glm.hpp>

#include "Environment.h"
#include "Manifold.h"
#include "PhysicsObject.h"

bool AABBvsAABB(Manifold* man) {
	bool collision = false;
	// pointers to objects
	Body* a = man->A;
	Body* b = man->B;

	// vector from A to B
	glm::vec3 n = b->position - a->position;

	AABB abox = a->getAABB();
	AABB bbox = b->getAABB();

	// calculate half extents along the x axis
	float a_extent = (abox.max.x - abox.min.x) / 2;
	float b_extent = (bbox.max.x - bbox.min.x) / 2;

	// calculate overlap on x
	float x_overlap = a_extent + b_extent - abs(n.x);

	// SAT test on x
	if (x_overlap > 0) {
		// calculate half extents along the y axis
		float a_extent = (abox.max.y - abox.min.y) / 2;
		float b_extent = (bbox.max.y - bbox.min.y) / 2;

		float y_overlap = a_extent + b_extent - abs(n.y);

		// SAT test on y
		if (y_overlap > 0) {
			// find the axis of least penetration
			if (x_overlap < y_overlap) {
				// point toward b since n points from a to b
				if (n.x < 0)
					man->normal = glm::vec3(-1.0f, 0.0f, 0.0f);
				else
					man->normal = glm::vec3(1.0f, 0.0f, 0.0f);
				man->penetration = x_overlap;
				collision = true;
			}
			else {
				// y is the least overlap
				// point toward b since n points from a to b
				if (n.y < 0)
					man->normal = glm::vec3(0.0f, -1.0f, 0.0f);
				else
					man->normal = glm::vec3(0.0f, 1.0f, 0.0f);
				man->penetration = y_overlap;
				collision = true;
			}
		}
	}

	return collision;
}

bool CirclevsCircle(Manifold* manifold) {
	bool collision = true;
	// pointers to objects
	Body* a = manifold->A;
	Body* b = manifold->B;

	// vector from A to B
	glm::vec3 n = b->position - a->position;

	//float r = a->radius + b->radius;
	float r = 1.0f;
	r *= r;

	// check if the circles in the manifold are actually colliding
	if (r < (n.x * n.x) + (n.y * n.y) + (n.z * n.z)) 
		collision = false;
	else {
		// if they are colliding

		// safely do a square root for the distance between the objects
		float distance = n.length();

		if (distance != 0) {
			manifold->penetration = r - distance;
		}
	}
	return collision;
}

bool collisionWithEnv(float envWidth, Body* body) {
	bool collision = false;
	// check floor
	if (body->position.y < (0.5 * body->scale.y)) {
		collision = true;
		float penetration = (0.5 * body->scale.y) - body->position.y;
		resolveCollision(glm::vec3(0.0f, 1.0f, 0.0f), body, penetration);
	}
	// check left wall
	if (body->position.x < (0.5 * body->scale.x)) {
		collision = true;
		float penetration = (0.5 * body->scale.x) - body->position.x;
		resolveCollision(glm::vec3(1.0f, 0.0f, 0.0f), body, penetration);
	}
	// check right wall
	if (body->position.x > envWidth - (0.5 * body->scale.x)) {
		collision = true;
		float penetration = body->position.x - (envWidth - (0.5 * body->scale.x));
		resolveCollision(glm::vec3(-1.0f, 0.0f, 0.0f), body, penetration);
	}

	return collision;
}

void resolveCollision(glm::vec3 normal, Body* body, float penetration) {
	glm::vec3 relativeVelocity = body->velocity;
	float veloctyAlongNormal = glm::dot(relativeVelocity, normal);

	// only continue if the objects are not separating
	if (veloctyAlongNormal < 0) {
		// calculate the restitution
		float epsilon = body->restitution;

		// calculate impulse scalar j and mass sum
		float j = -(1 + epsilon) * veloctyAlongNormal;
		j /= body->inverseMass;

		// apply impulse
		// always subtract from a and add to b since the normal points from a to b
		glm::vec3 impulse = j * normal;
		body->velocity += body->inverseMass * impulse;

	}
	// positional correction
	positionalCorrection(normal, body, penetration);
}

void resolveCollision(Manifold* manifold) {
	// pointers to objects
	Body* a = manifold->A;
	Body* b = manifold->B;

	glm::vec3 relativeVelocity = b->velocity - a->velocity;
	float veloctyAlongNormal = glm::dot(relativeVelocity, manifold->normal);

	// only continue if the objects are not separating
	if (veloctyAlongNormal < 0) {
		// calculate the restitution
		float epsilon = std::min(a->restitution, b->restitution);

		// calculate impulse scalar j and mass sum
		float j = -(1 + epsilon) * veloctyAlongNormal;
		j /= a->inverseMass + b->inverseMass;

		// apply impulse
		// always subtract from a and add to b since the normal points from a to b
		glm::vec3 impulse = j * manifold->normal;
		a->velocity -= a->inverseMass * impulse;
		b->velocity += b->inverseMass * impulse;
	}
	// positional correction
	manifold->PositionalCorrection();
}

void positionalCorrection(glm::vec3 normal, Body* body, float penetration) {
	const float percent = 1.0; // usually 20% to 80%
	const float slop = 0.01; // usually 0.01 to 0.1
	glm::vec3 correction = (std::max(penetration - slop, 0.0f) / (body->inverseMass)) * percent * normal;
	body->position += body->inverseMass * correction;
}
