#include <glm/glm.hpp>
#include "VectorUtils.h"

glm::vec3 vectorUtils::add(glm::vec3 vector1, glm::vec3 vector2) {
        
    float xSum = vector1.x + vector2.x;
    float ySum = vector1.y + vector2.y;
    float zSum = vector1.z + vector2.z;
    glm::vec3 resultant(xSum, ySum, zSum);

    return resultant;
}


glm::vec3 vectorUtils::sub(glm::vec3 vector1, glm::vec3 vector2) {
        
    float xDifference = vector1.x - vector2.x;
    float yDifference = vector1.y - vector2.y;
    float zDifference = vector1.z - vector2.z;
    glm::vec3 resultant(xDifference, yDifference, zDifference);

    return resultant;
}


glm::vec3 vectorUtils::scalarMult(glm::vec3 vector, float scalar) {

    float xProduct = vector.x * scalar;
    float yProduct = vector.y * scalar;
    float zProduct = vector.z * scalar;
    glm::vec3 scaledVector(xProduct, yProduct, zProduct);

    return scaledVector;
}
    

float vectorUtils::dotProduct(glm::vec3 vector1, glm::vec3 vector2) {

	float xProduct = vector1.x * vector2.x;
	float yProduct = vector1.y * vector2.y;
    float zProduct = vector1.z * vector2.z;
	float productScalar = xProduct + yProduct + zProduct;

	return productScalar;
}
