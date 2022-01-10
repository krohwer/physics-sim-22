#pragma once

#ifndef COLLISION_H
#define COLLISION_H

#include <glm/glm.hpp>
#include "PhysicsObject.h"
#include "Manifold.h"

struct Pair {
	Body* A;
	Body* B;
};

/// detect a collision between two bounding boxes
void BoxvsBox(Manifold *manifold);

/// detect a collision between two balls
void BallvsBall(Manifold* manifold);

// TODO: Find axis least penetration

void BoxvsBall(Manifold* manifold);
void BallvsBox(Manifold* manifold);

typedef void (*collisionCallback)(Manifold* man);

extern collisionCallback Dispatch[2][2];

#endif