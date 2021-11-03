#pragma once

#ifndef COLLISION_H
#define COLLISION_H

#include <algorithm>
#include <glm/glm.hpp>
#include "PhysicsObject.h"

struct Manifold;
struct Body;

/// detect a collision between two axis aligned bounding boxes
bool AABBvsAABB(Manifold *man);

// CIRCLES

struct Circle {
	float radius;
	glm::vec3 position;
};

// detect a collision between two circles
bool CirclevsCircle(Manifold* man);

void resolveCollision(Body a, Body b) {
	glm::vec3 relativeVelocity = b.velocity - a.velocity;
	//float veloctyAlongNormal = glm::dot(relativeVelocity, normal);

	//float epsilon = std::min()
}

//typedef void (*CollisionCallback)(Manifold* m, Body* a, Body* b);

//extern CollisionCallback Dispatch[Shape::eCount][Shape::eCount];

void CircletoCircle(Manifold* m, Body* a, Body* b);
void CircletoPolygon(Manifold* m, Body* a, Body* b);
void PolygontoCircle(Manifold* m, Body* a, Body* b);
void PolygontoPolygon(Manifold* m, Body* a, Body* b);

#endif