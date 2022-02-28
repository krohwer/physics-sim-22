#pragma once

#ifndef PHYSICSMATH_H
#define PHYSICSMATH_H

#include <glm/glm.hpp>

// conversion factor from meters to pixels
#define PIXEL_RATIO 100.0f
// default value for gravity
#define DEFAULT_GRAVITY 9.81f

#define PI 3.14159f
#define toRadians PI / 180.0f
#define toDegrees 180.0f / PI

// physics timing constants
const float physicsFPS = 200;
const float timestep = 1 / physicsFPS;

// rounding tolerance value
const float EPSILON = 0.0001f;

// length squared, provides the squared magnitude of a vector
inline float lenSqr(glm::vec3 vector) {
	return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
}

// Two crossed (2D) vectors return a scalar
inline float CrossProduct(glm::vec3 a, glm::vec3 b) {
	return a.x * b.y - a.y * b.x;
}

// Comparison with tolerance of EPSILON
inline bool Equal(float a, float b)
{
	// <= instead of < for NaN comparison safety
	return std::abs(a - b) <= EPSILON;
}

#endif