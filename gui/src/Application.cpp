#include <GL/glew.h> // must be included before other OpenGL Libraries
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define ASSERT(x) if (!(x)) __debugbreak();

// read the shader file and convert it into a string
static std::string ParseShader(const std::string& filepath) {
	
	std::ifstream stream(filepath);

	std::string line;
	std::stringstream shaderString;
	while (getline(stream, line)) {
		shaderString << line << '\n';
	}

	// return the shader as a string
	return shaderString.str();
}

// tells OpenGL to compile our shaders
static unsigned int CompileShader(unsigned int type, const std::string& source) {

	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();	// pointer to the first character of the string
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	// Error Handling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		// alloca() lets you allocate on the stack dynamically
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

// provides OpenGL with our shader source code
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

	// unsigned int vs gluint: same thing, but using raw c++ is just compatible with other APIs
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main(void)
{
	// GLFW INITIALIZATION //

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Set the version of OpenGL to 3.3 */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	/* Set the profile to CORE */
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// waits for 1 screen update to swap the interval, results in basically vsync!
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "GLEW is not OK!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;
	
	// DEFINING THE OBJECT TO RENDER //

	// each line is a vertex position in the form x, y
	float positions[] = {
		-0.5f, -0.5f,	// 0
		 0.5f, -0.5f,	// 1
		 0.5f,  0.5f,	// 2
		-0.5f,  0.5f	// 3
	};

	// this is an index buffer.  It tells OpenGL how to draw a square without storing duplicate vertices
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	// INITIAL VERTEX ARRAY OBJECT AND BUFFER CREATION //

	// create and bind our vertex array object
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// create a vertex buffer
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW);
	
	// REMEMBER to enable this or it won't work.  The index is that of the attribute
	glEnableVertexAttribArray(0);
	// Once the buffer is bound, we need to tell OpenGL what the layout of the buffer is
	// Need one of these for each attribute of a vertex (position, texture, color, normal, etc)
	// Parameters: index is the beginning position of the attribute in the vertex
	//			   size is the the component count of the attribute
	//			   the type is the type of data
	//			   do you want them normalized?
	//			   stride amount you need to go forward to get to the next vertex
	//			   pointer is the amount to the next attribute.  If you have a struct for your vertex, you can use offsetof()
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);	// with this line, we say index 0 of this vertex array is bound to the currently bound array buffer.  This links the buffer to the VAO.  This can be used to bind multiple vertex buffers to a single VAO.

	// create an index buffer
	unsigned int ibo;	// index buffer object
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// LOAD SHADERS //

	std::string vertexSource = ParseShader("resources/shaders/Vertex.shader");
// 	std::cout << "VERTEX SHADER" << std::endl;
// 	std::cout << vertexSource << std::endl;
	std::string fragmentSource = ParseShader("resources/shaders/Fragment.shader");
// 	std::cout << "FRAGMENT SHADER" << std::endl;
// 	std::cout << fragmentSource << std::endl;

	unsigned int shader = CreateShader(vertexSource, fragmentSource);
	glUseProgram(shader);

	//retrieve the location of the uniform we are using for color
	int location = glGetUniformLocation(shader, "u_Color");
	// this assert is optional since we may not use color, but I figured it's important to include for now
	ASSERT(location != -1);
	// glUnform 4 f because for a vec4 we need 4 floats
	glUniform4f(location, 0.3f, 0.3f, 0.8f, 1.0f);


	// unbind the buffers, to test use of vertex array object
	glBindVertexArray(0);
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	// variables to represent red and how much we want it to change each tick
	float r = 0.0f;
	float increment = 0.05f;

	// RENDER LOOP //

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		// bind the shader
		glUseProgram(shader);
		// sending r in as red to animate the color
		glUniform4f(location, r, 0.3f, 0.8f, 1.0f);

		// before drawing, bind the vertex array, and the index buffer
		// this adjustment is helpful for rendering multiple objects
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		// still drawing in triangles, then the number of INDICES (not vertices), and the data type of the indices.
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		// change r to animate the color of the object
		if (r > 1.0)
			increment = -0.05f;
		else if (r < 0.0f)
			increment = 0.05f;

		r += increment;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	// delete the shader
	glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}