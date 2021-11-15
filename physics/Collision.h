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

/// detect a collision between two circles
bool CirclevsCircle(Manifold* man);

/**
 * Detect a collision between a physics body and the environment bounds, and automatically resolve it
 */
bool collisionWithEnv(float envWidth, Body* body);

/**
 * Resolve a collision between a single physics body and the environment
 * 
 * @param normal - The normal vector of the environment bound (straight up for the floor)
 * @param body - The body to resolve
 * @param penetration - The penetration depth of the body into the floor/wall
 */
void resolveCollision(glm::vec3 normal, Body* body, float penetration);

/**
 * Resolve a collision between two physics bodies in a manifold
 */
void resolveCollision(Manifold* man);

/**
 * Corrects sinking of a body based on the normal vector and the penetration depth
 */
void positionalCorrection(glm::vec3 normal, Body* body, float penetration);

//typedef void (*CollisionCallback)(Manifold* m, Body* a, Body* b);

//extern CollisionCallback Dispatch[Shape::eCount][Shape::eCount];

void CircletoCircle(Manifold* m, Body* a, Body* b);
void CircletoPolygon(Manifold* m, Body* a, Body* b);
void PolygontoCircle(Manifold* m, Body* a, Body* b);
void PolygontoPolygon(Manifold* m, Body* a, Body* b);

#endif