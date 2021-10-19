# physics-sim-22
Physics Simulator 2022

## Opening This Code With VS
To open this code for editing and compiling, first open Visual Studio and clone the repo. 
In your Solution Explorer, you should see a file titled "physics-sim-22.sln" which is the solution file for this project. 
Double click this to open the solution, and then click the button that looks like 3 files, to show all files. (you may have to show all files within projects as well)
You can now open and edit any of these files.

## Contributing to This Project With VS
For each individual component that you wish to add, click File > New > Project... and in the dialog box select Empty Project.
On the next screen you will be given options to name the project.  Name it something appropriate for a directory (I.E. vector for the vector component) 
and make sure under "Solution" you select the option "Add to solution". This should automatically select physics-sim-22.
Click create to add your project, then you can create files by right clicking, selecting Add > New Item and choosing the appropriate format.

## Configuring and Compiling
So in general, all of our projects except the one that contains our main() and runs the whole program will be configured as Static Libraries. 
To do this, right click on the project and select "Properties". Make sure at the top of the window you have "All Configurations" and "All Platforms" selected. 
Then under Configuration Properties > General, change the Configuration Type to Static library (.lib)

In addition to this, in the Properties of the project containing main(), you'll need to add the project directory to the "Additional Include Directories" under C/C++ > General.
You can add this in the format $(SolutionDir)name_of_project_folder.
This will allow you to simply #include "project_name" in the main file instead of doing some shenanigans with including the directory in the include statement.
this *should* compile, but it gets funky sometimes, so let me know if you're having issues and we'll work it out. I was having some weird issues and had to register some stuff.

Finally, we will be compiling all of our things into 32bit (x86) windows applications. On the top bar next to the windows debugger button, be sure to set the options to debug and x86.

## Syntax, Styling, and Comments
### Syntax
Just a quick note about some of the weirder syntax quirks of C++.
Class definitions should end the curly brace with a semicolon, while function definitions should not:
```C++
class MyClass {
	...
};	// semicolon

void myFunction() {

}	// no semicolon
```
Classes should be defined inside a header (.h) file and should contain method stubs:
```C++
class MyClass {
public:
	int x;

	void myFunction(parameter);
}
```
The function will then be defined inside the corressponding .cpp file like so:
```C++
void MyClass::myFunction(parameter) {
	...
}
```
Functions that are not contained within classes follow the same rules.  Stubs in the header, then full definitions in the .cpp file.  The difference is you do not need to specify the scope (`::`).

The format of a main function is:
```C++
int main() {
	...
	return 0;
}
```

### Styling
In C++, the typical naming conventions are: 
* Classes: `UpperCamelCase`
* Functions: `lowerCamelCase`
* Variables: `lowerCamelCase`

### Comments
The general rule for comments is to put detailed block comments (`/**`) in your header file with your definitions, and then functional comments (`//`) with your actual code. For example, this would go in the header:
```C++
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
```
The middle section of this comment is optional if the purpose/use is self explanatory, just include a brief definition of what it does.

Then you can comment your code however you like in your .cpp file. See Vector.h and Vector.cpp for more examples.

## Vectors
Our previous vector class has now been completely deprecated, as glm's vec3 does literally everything we need and more. 
I'm basically just leaving it around for documentations sake, and it will not be compiled into our final product.

To use vectors, make sure your project has `$(SolutionDir)Dependencies` in its additional include directories under properties, and then use `#include <glm/glm.hpp>`

### Vector Operations
glm has basically all the functions we'll need for using their vectors and more. Here is a nice summary:
```C++
// adding vectors:
glm::vec3 vectorSum = vector1 + vector2;
// subtracting vectors:
glm::vec3 vectorDiff = vector1 - vector2;
// magnitude of a vector:
float magnitude = glm::length(vector);
// dot product:
float dotProduct = glm::dot(vector1, vector2);
// cross product:
float crossProduct = glm::cross(vector1, vector2);
// reflection:
glm::vec3 result = glm::reflect(incidentVector, surfaceNormalVector);
```