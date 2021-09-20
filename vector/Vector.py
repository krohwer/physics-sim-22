import math


class Vector:
    """
    Basic vector class for use as position, velocity, or acceleration.
    In usage, the tail will always be placed at the origin of the object the vector is applied to.
    Position vectors will always be placed at the origin of the coordinate plane.

    Functions:
        getX()
        getY()
        getMagnitude()
        add(second_vector)
        sub(second_vector)
        dotProduct(second_vector)
        scalarMult(scalar)
    """
    def __init__(self, x_coordinate, y_coordinate):
        self.x = x_coordinate
        self.y = y_coordinate

    def getX(self):
        """
        Gets the X coordinate of the vector's head with the tail placed at the origin

        :return: (float)
        """
        return self.x

    def getY(self):
        """
        Gets the Y coordinate of the vector's head with the tail placed at the origin

        :return: (float)
        """
        return self.y

    def getMagnitude(self):
        """
        Gets the magnitude of the vector

        :return: (float)
        """
        magnitude = math.sqrt((self.x ** 2) + (self.y ** 2))
        return magnitude

    def add(self, second_vector):
        """
        Adds second_vector to the self vector

        :param second_vector: (Vector) added to the self vector
        :return: (Vector)
        """
        x_sum = self.x + second_vector.getX()
        y_sum = self.y + second_vector.getY()
        sum_vector = Vector(x_sum, y_sum)
        return sum_vector

    def sub(self, second_vector):
        """
        Subtracts second_vector from the self vector

        :param second_vector: (Vector) subtracted from the self vector
        :return: (Vector)
        """
        x_difference = self.getX() - second_vector.getX()
        y_difference = self.getY() - second_vector.getY()
        difference_vector = Vector(x_difference, y_difference)
        return difference_vector

    def dotProduct(self, second_vector):
        """
        Multiplies the self vector by given vector using the dot product

        :param second_vector: (Vector) projected onto the self vector
        :return: (float)
        """
        # TODO: A dot B = |A||B|cos(Theta) where Theta is the angle between the two vectors
        product_scalar = 0
        return product_scalar

    def scalarMult(self, scalar):
        """
        Multiplies the vector by a floating point scalar value

        :param scalar: (float) scalar value multiplying the vector
        :return: (Vector)
        """
        x_product = self.getX() * scalar
        y_product = self.getY() * scalar
        scaled_vector = Vector(x_product, y_product)
        return scaled_vector
