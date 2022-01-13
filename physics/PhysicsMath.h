#pragma once

#ifndef PHYSICSMATH_H
#define PHYSICSMATH_H

#include <glm/glm.hpp>

// physics timing constants
const float physicsFPS = 200;
const float timestep = 1 / physicsFPS;

// gravity constant
const float GRAVITY = 9.81f;

// rounding tolerance value
const float EPSILON = 0.0001f;

/**
 * A rotation matrix that defines counter-clockwise rotation
 * Usage: generate the matrix based on the angle in radians, then multiply it by a vector
 */
struct mat22 {
	float m00, m01,
		  m10, m11;

	mat22() {}

	mat22(float xx, float xy, float yx, float yy) {
		m00 = xx; m01 = xy;
		m10 = yx; m11 = yy;
	}

	mat22(glm::vec3 x, glm::vec3 y) {
		m00 = x.x; m01 = x.y;
		m10 = y.x; m11 = y.y;
	}

	mat22(float radians) {
		float c = std::cos(radians);
		float s = std::sin(radians);

		m00 = c; m01 = -s;
		m10 = s; m11 = c;
	}

	void set(float radians) {
		float c = std::cos(radians);
		float s = std::sin(radians);

		m00 = c; m01 = -s;
		m10 = s; m11 = c;
	}

	glm::vec3 AxisX(void) {
		return glm::vec3(m00, m10, 0.0f);
	}

	glm::vec3 AxisY(void) {
		return glm::vec3(m01, m11, 0.0f);
	}

	mat22 Transpose(void) {
		return mat22(m00, m10, m01, m11);
	}

	// Rotate a vector
	const glm::vec3 operator*(const glm::vec3 rhs) const
	{
		return glm::vec3(m00 * rhs.x + m01 * rhs.y, m10 * rhs.x + m11 * rhs.y, 0.0f);
	}
};

// length squared, provides the squared magnitude of a vector
inline float lenSqr(glm::vec3 vector) {
	return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
}

// Two crossed (2D) vectors return a scalar
inline float CrossProduct(glm::vec3 a, glm::vec3 b) {
	return a.x * b.y - a.y * b.x;
}

// More exotic (but necessary) forms of the cross product
// with a vector a and scalar s, both returning a vector
inline glm::vec3 CrossProduct(glm::vec3 a, float scalar) {
	return glm::vec3(scalar * a.y, -scalar * a.x, 0.0f);
}

inline glm::vec3 CrossProduct(float scalar, glm::vec3 a) {
	return glm::vec3(-scalar * a.y, scalar * a.x, 0.0f);
}

// Comparison with tolerance of EPSILON
inline bool Equal(float a, float b)
{
	// <= instead of < for NaN comparison safety
	return std::abs(a - b) <= EPSILON;
}

#endif