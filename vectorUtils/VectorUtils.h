#pragma once

#include <glm/glm.hpp>

#ifndef VECTORUTILS_H // header guards
#define VECTORUTILS_H

/**
 * [DEPRECATED]
 * Basic Vector operations using glm::vec3 (z = 0 in 2D space)
 * In usage, the tail will always be placed at the origin of the object the vector is applied to.
 * Position vectors will always be placed at the origin of the coordinate plane.
 * Does not include magnitude.  Use glm::length(vector)
 */
namespace vectorUtils {

	/**
	 * Adds two Vectors to obtain the resultant, a Vector that represents the combined effect of the two input Vectors.
	 * The input Vectors are order independent.
	 * 
	 * @param vector1, vector2 - The Vectors to be added
	 */
	glm::vec3 add(glm::vec3 vector1, glm::vec3 vector2);

	/**
	 * subtracts two Vectors to get the resultant, a Vector that represents the combined effect of the self Vector and the negation of the second Vector.
	 * 
	 * @param vector1 - The first Vector in the subtraction
	 * @param vector2 - The negated Vector in the subtraction
	 */
	glm::vec3 sub(glm::vec3 vector1, glm::vec3 vector2);

	/**
	 * Multiplies the components of a Vector by a floating point scalar value in order to 'scale' the Vector.
	 * 
	 * @param vector - The Vector to be scaled
	 * @param scalar - a double value to multiply the components of a Vector
	 */
	glm::vec3 scalarMult(glm::vec3 vector, float scalar);

	/**
	 * Calculates the dot product of two Vectors.
	 * 
	 * The dot product of Vectors essentially tells how much of the second Vector is being applied in the direction of the first Vector.
	 * This can be used to calculate work when the force and displacement Vectors have different directions, or to help calculate Theta, the angle between two Vectors.
	 * 
	 * @param vector1 - The Vector that determines the direction
	 * @param vector2 - The Vector that is measured in the direction of the first Vector
	 */
	float dotProduct(glm::vec3 vector1, glm::vec3 vector2);
}

// ADDITIONAL NOTES //
// eventually might need a function to get the smallest angle between vectors.
// cross product of vectors would be useful if moved into 3D space, as it allows us to get the vector perpendicular to the plane of the two vectors involved.

#endif