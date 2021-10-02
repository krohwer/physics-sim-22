#pragma once
#ifndef VECTOR_H
#define VECTOR_H

/**
 * Basic Vector class for use as position, displacement, velocity, or acceleration.
 * In usage, the tail will always be placed at the origin of the object the vector is applied to.
 * Position vectors will always be placed at the origin of the coordinate plane.
 */
class Vector {
public:
	/// x, y are the x and y coordinate of the vector head when the tail is placed at the origin
	double x, y;

	/**
	 * Default constructor for the Vector class.
	 * Constructs the zero Vector.
	 */
	Vector();

	/**
	 * Secondary constructor for the Vector class.
	 * Constructs a Vector.
	 * 
	 * @param xComponent - The desired X component of the Vector
	 * @param yComponent - The desired Y component of the Vector
	 */
	Vector(double xComponent, double yComponent);

	/**
	 * Alters the X component of the Vector.
	 * 
	 * @param xComponent - The desired X component of the Vector
	 */
	void setX(double xComponent);

	/**
	 * Alters the Y component of the Vector.
	 * 
	 * @param yComponent - The desired Y component of the Vector
	 */
	void setY(double yComponent);

	/**
	 * Gets the magnitude of the Vector, which is the distance from tail to head.
	 */
	double magnitude();
};

/**
 * Adds two Vectors to obtain the resultant, a Vector that represents the combined effect of the two input Vectors.
 * The input Vectors are order independent.
 * 
 * @param vector1, vector2 - The Vectors to be added
 */
Vector add(Vector vector1, Vector vector2);

/**
 * subtracts two Vectors to get the resultant, a Vector that represents the combined effect of the self Vector and the negation of the second Vector.
 * 
 * @param vector1 - The first Vector in the subtraction
 * @param vector2 - The negated Vector in the subtraction
 */
Vector sub(Vector vector1, Vector vector2);

/**
 * Multiplies the components of a Vector by a floating point scalar value in order to 'scale' the Vector.
 * 
 * @param vector - The Vector to be scaled
 * @param scalar - a double value to multiply the components of a Vector
 */
Vector scalarMult(Vector vector, double scalar);

/**
 * Calculates the dot product of two Vectors.
 * 
 * The dot product of Vectors essentially tells how much of the second Vector is being applied in the direction of the first Vector.
 * This can be used to calculate work when the force and displacement Vectors have different directions, or to help calculate Theta, the angle between two Vectors.
 * 
 * @param vector1 - The Vector that determines the direction
 * @param vector2 - The Vector that is measured in the direction of the first Vector
 */
double dotProduct(Vector vector1, Vector vector2);

// ADDITIONAL NOTES //
// eventually might need a function to get the smallest angle between vectors.
// cross product of vectors would be useful if moved into 3D space, as it allows us to get the vector perpendicular to the plane of the two vectors involved.

#endif