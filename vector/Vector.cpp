#include "Vector.h"
#include <math.h>
/*
    Basic vector class for use as position, displacement, velocity, or acceleration.
    In usage, the tail will always be placed at the origin of the object the vector is applied to.
    Position vectors will always be placed at the origin of the coordinate plane.

    data:
    double x
    double y

    Functions:
    double magnitude()
*/

// constructor
Vector::Vector(double xCoordinate, double yCoordinate) {
    x = xCoordinate;
    y = yCoordinate;
};

/*
    Gets the magnitude of the vector, which is the distance from tail to head
*/
double Vector::magnitude() {

    return sqrt(pow(x, 2) + pow(y, 2));
}

// FUNCTIONS

/*
    adds the vectors to get the resultant, a vector that represents the combined effect of the two vectors
*/
Vector add(Vector vector1, Vector vector2) {
        
    double xSum = vector1.x + vector2.x;
    double ySum = vector1.y + vector2.y;
    Vector resultant = Vector(xSum, ySum);

    return resultant;
}


/*
    subtracts the vectors to get the resultant, a vector that represents the combined effect of the self vector and the negation of the second vector
*/
Vector sub(Vector vector1, Vector vector2) {
        
    double xDifference = vector1.x - vector2.x;
    double yDifference = vector1.y - vector2.y;
    Vector resultant = Vector(xDifference, yDifference);

    return resultant;
}

/*
    Multiplies the vector by a floating point scalar value in order to 'scale' the vector
*/
Vector scalarMult(Vector vector, double scalar) {

    double xProduct = vector.x * scalar;
    double yProduct = vector.y * scalar;
    Vector scaledVector = Vector(xProduct, yProduct);

    return scaledVector;
}
    
/*
	The dot product of vectors essentially tells how much of the second vector is being applied in the direction of the self vector.
	This can be used to calculate work when the force and displacement vectors have different directions, or to help calculate Theta, the angle between two vectors
*/
double dotProduct(Vector vector1, Vector vector2) {

	double xProduct = vector1.x * vector2.x;
	double yProduct = vector1.y * vector2.y;
	double productScalar = xProduct + yProduct;

	return productScalar;
}

/// ADDITIONAL NOTES ///
// eventually might need a function to get the smallest angle between vectors.
// cross product of vectors would be useful if moved into 3D space, as it allows us to get the vector perpendicular to the plane of the two vectors involved.
