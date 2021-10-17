#include "Vector.h"
#include <math.h>


Vector::Vector() {
    x = 0;
    y = 0;
}


Vector::Vector(float xComponent, float yComponent) {
    x = xComponent;
    y = yComponent;
};


void Vector::setX(float xComponent) {
    x = xComponent;
}


void Vector::setY(float yComponent) {
    y = yComponent;
}


float Vector::magnitude() {

    return sqrt(x*x + y*y);
}


// EXTERNAL FUNCTIONS

Vector add(Vector vector1, Vector vector2) {
        
    float xSum = vector1.x + vector2.x;
    float ySum = vector1.y + vector2.y;
    Vector resultant = Vector(xSum, ySum);

    return resultant;
}


Vector sub(Vector vector1, Vector vector2) {
        
    float xDifference = vector1.x - vector2.x;
    float yDifference = vector1.y - vector2.y;
    Vector resultant = Vector(xDifference, yDifference);

    return resultant;
}


Vector scalarMult(Vector vector, float scalar) {

    float xProduct = vector.x * scalar;
    float yProduct = vector.y * scalar;
    Vector scaledVector = Vector(xProduct, yProduct);

    return scaledVector;
}
    

float dotProduct(Vector vector1, Vector vector2) {

	float xProduct = vector1.x * vector2.x;
	float yProduct = vector1.y * vector2.y;
	float productScalar = xProduct + yProduct;

	return productScalar;
}
