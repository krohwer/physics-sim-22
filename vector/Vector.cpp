#include "Vector.h"
#include <math.h>


Vector::Vector() {
    x = 0;
    y = 0;
}


Vector::Vector(double xComponent, double yComponent) {
    x = xComponent;
    y = yComponent;
};


void Vector::setX(double xComponent) {
    x = xComponent;
}


void Vector::setY(double yComponent) {
    y = yComponent;
}


double Vector::magnitude() {

    return sqrt(x*x + y*y);
}


// EXTERNAL FUNCTIONS

Vector add(Vector vector1, Vector vector2) {
        
    double xSum = vector1.x + vector2.x;
    double ySum = vector1.y + vector2.y;
    Vector resultant = Vector(xSum, ySum);

    return resultant;
}


Vector sub(Vector vector1, Vector vector2) {
        
    double xDifference = vector1.x - vector2.x;
    double yDifference = vector1.y - vector2.y;
    Vector resultant = Vector(xDifference, yDifference);

    return resultant;
}


Vector scalarMult(Vector vector, double scalar) {

    double xProduct = vector.x * scalar;
    double yProduct = vector.y * scalar;
    Vector scaledVector = Vector(xProduct, yProduct);

    return scaledVector;
}
    

double dotProduct(Vector vector1, Vector vector2) {

	double xProduct = vector1.x * vector2.x;
	double yProduct = vector1.y * vector2.y;
	double productScalar = xProduct + yProduct;

	return productScalar;
}
