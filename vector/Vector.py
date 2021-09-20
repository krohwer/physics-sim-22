import math


class Vector:
    """
    Basic vector class for use as position, displacement, velocity, or acceleration.
    In usage, the tail will always be placed at the origin of the object the vector is applied to.
    Position vectors will always be placed at the origin of the coordinate plane.

    Functions:
        getX()
        getY()
        getMagnitude()
        add(second_vector)
        sub(second_vector)
        scalarMult(scalar)
        dotProduct(second_vector)
    """
    ### CONSTRUCTORS ###

    def __init__(self, x_coordinate, y_coordinate):
        self.x = x_coordinate
        self.y = y_coordinate

    ### OVERRIDES ###

    def __eq__(self, other):
        return isinstance(other, Vector) and self.getX() == other.getX() and self.getY() == other.getY()

    def __ne__(self, other):
        return not self == other

    ### FUNCTIONS ###

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
        Gets the magnitude of the vector, which is the distance from tail to head

        :return: (float)
        """
        magnitude = math.sqrt((self.x ** 2) + (self.y ** 2))
        return magnitude

    def add(self, second_vector):
        """
        Adds the vectors to get the resultant, a vector that represents the combined effect of the two vectors

        :param second_vector: (Vector)
        :return: (Vector)
        """
        x_sum = self.x + second_vector.getX()
        y_sum = self.y + second_vector.getY()
        resultant = Vector(x_sum, y_sum)
        return resultant

    def sub(self, second_vector):
        """
        subtracts the vectors to get the resultant, a vector that represents the combined effect of the self vector
        and the negation of the second vector

        :param second_vector: (Vector)
        :return: (Vector)
        """
        x_difference = self.getX() - second_vector.getX()
        y_difference = self.getY() - second_vector.getY()
        resultant = Vector(x_difference, y_difference)
        return resultant

    def scalarMult(self, scalar):
        """
        Multiplies the vector by a floating point scalar value in order to 'scale' the vector

        :param scalar: (float)
        :return: (Vector)
        """
        x_product = self.getX() * scalar
        y_product = self.getY() * scalar
        scaled_vector = Vector(x_product, y_product)
        return scaled_vector

    def dotProduct(self, second_vector):
        """
        The dot product of vectors essentially tells how much of the second vector is being applied in the direction of
        the self vector.
        This can be used to calculate work when the force and displacement vectors have different directions, or
        to help calculate Theta, the angle between two vectors

        :param second_vector: (Vector)
        :return: (float)
        """
        x_product = self.getX() * second_vector.getX()
        y_product = self.getY() * second_vector.getY()
        product_scalar = x_product + y_product
        return product_scalar

    ### ADDITIONAL NOTES ###
    # eventually might need a function to get the smallest angle between vectors.
    # cross product of vectors would be useful if moved into 3D space, as it allows us to get the vector perpendicular
    # to the plane of the two vectors involved.
