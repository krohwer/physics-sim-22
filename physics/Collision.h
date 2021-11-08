#pragma once

#ifndef COLLISION_H
#define COLLISION_H

#include <glm/glm.hpp>

struct Manifold;
struct Body;
class Environment;

/// detect a collision between two axis aligned bounding boxes
bool AABBvsAABB(Manifold *man);

// CIRCLES

struct Circle {
	float radius;
	glm::vec3 position;
};

// detect a collision between two circles
bool CirclevsCircle(Manifold* man);

bool collisionWithEnv(float envWidth, Body* body);

void resolveCollision(glm::vec3 normal, Body* body, float penetration);

void resolveCollision(Manifold* man);

void positionalCorrection(glm::vec3 normal, Body* body, float penetration);

//typedef void (*CollisionCallback)(Manifold* m, Body* a, Body* b);

//extern CollisionCallback Dispatch[Shape::eCount][Shape::eCount];

void CircletoCircle(Manifold* m, Body* a, Body* b);
void CircletoPolygon(Manifold* m, Body* a, Body* b);
void PolygontoCircle(Manifold* m, Body* a, Body* b);
void PolygontoPolygon(Manifold* m, Body* a, Body* b);

#endif