#include "pch.h"
#include <GLFW/glfw3.h> // also uses glew 2.1.0 in precompiled header
#include <glm/glm.hpp> // glm version 9.9.8
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

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
	int windowWidth = 960;
	int windowHeight = 540;
	window = glfwCreateWindow(windowWidth, windowHeight, "Hello World", NULL, NULL);
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

	{ // this is a scope to prevent errors when we end our GL context by calling our destructors at end of scope

		// each line is a vertex position in the form x, y
		// since we have a projection matrix set up now, 0,0 is the bottom left of the screen
		// NOTE: we want to define these vertex positions as centered around 0,0 for our object coordinates
		//		 Then we'll use our model matrix to actually place it in *world* coordinates
		//		 This data will probably need to be stored in the object class
		float positions[] = {
			-50.0f, -50.0f,	// 0
			 50.0f, -50.0f,	// 1
			 50.0f,  50.0f,	// 2
			-50.0f,  50.0f	// 3
		};

		/*
		BIG NOTE:
		Since we are working with very basic objects and we will have set types, one way to go about rendering multiple of these is to predefine default objects, and store either an entire model matrix, or simply the vec3's that we need to calculate the model matrix for each given object.  These vec3's being position, rotation, and scale. Things that we would probably want to be storing for each object anyway.
		This means that each time we want to render a square, we simply use the default vertex array for a square, apply the object's transformations to the model matrix, set the uniform, and then draw the object. We would then also need a default vertex array for a circle, and any other shape/sprite that we wish to render.
		What I'm not sure about is how this will affect collisions, since I guess we will need to somehow get the "model" of the object to check if it intersects with anything. 

		Finally if we do need to use vec3s to store that data, that means we'll be importing glm in the PhysicsObject class anyway, so we might as well actually change vectorUtils to use vec3's like was mentioned before, and just use glm vectors for everything. Oh joy. I realized I set stuff up so this isn't that difficult, because the dependencies directory is actually in the solution folder, not the gui project folder. 
		*/

		// this is an index buffer.  It tells OpenGL how to draw a square without storing duplicate vertices
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		// INITIAL VERTEX ARRAY OBJECT AND BUFFER CREATION //

		// create and bind our vertex array object
		VertexArray va;
		VertexBuffer vb(positions, 4 * 2 * sizeof(float));

		VertexBufferLayout layout;
		// this can be expanded on later and include many types including vec2s or vec3s when it comes to positions
		layout.push<float>(2);	// this means we have 2 floats for a vertex
		va.addBuffer(vb, layout);

		// create an index buffer and automatically bind it
		IndexBuffer ib(indices, 6);

		// CREATE PROJECTION MATRIX //

		glm::mat4 projection = glm::ortho(0.0f, (float)windowWidth, 0.0f, (float)windowHeight, -1.0f, 1.0f);

		// LOAD SHADERS //

		// create and bind the shader
		Shader shader("resources/shaders/Vertex.shader", "resources/shaders/Fragment.shader");
		shader.bind();

		// SET UNIFORMS //

		// set the color uniform for the shader
		shader.setUniform4f("u_Color", 0.3f, 0.3f, 0.8f, 1.0f);

		// UNBIND EVERYTHING //

		// unbind the buffers, to test use of vertex array object
		va.unbind();
		vb.unbind();
		ib.unbind();
		shader.unbind();

		// create a renderer
		Renderer renderer;

		// variables to represent red and how much we want it to change each tick
		float r = 0.0f;
		float increment = 0.01f;

		// RENDER LOOP //

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */

			renderer.clear();

			// bind the shader
			// in a perfect world, you do this right before you actually draw an object, and have a shader cache to make sure shaders are not bound multiple times
			shader.bind();

			{ // SCOPE TO CALCULATE MVP MATRIX AND DRAW AN OBJECT //

				// identity model matrix
				// can use it to move the object with glm::translate(model, glm::vec3(xOffset, yOffset, zOffset));
				// the vec3 you're sending in is basically the displacement vector, telling how much the object moves in each direction
				// for multiple objects, we will need a separate model matrix for each one since it converts from "object" coordinates to "world" coordinates
				glm::mat4 model(1.0f);
				/* DO ANY MODEL MATRIX TRANSFORMATIONS */
				// translations, rotations, and scaling

				// multiply the model, view, and projection matrices in reverse order to create the mvp.  We're kinda ignoring the view matrix since we'll use a static camera
				// NOTE: even though matrices are quick to run on the GPU, we want to be doing these kinds of calculations on the CPU because they DO NOT need to run for every vertex.
				//		 Only put things in the shader that *need* to run for every single vertex
				glm::mat4 mvp = projection * model;

				// set the matrix uniform for the mvp matrix so it updates every frame
				shader.setUniformMat4f("u_MVP", mvp);

				// call the renderer to draw something
				// send in a vertex array, an index buffer, and a shader
				// in a more traditional setup, we would be using a material instead of a shader
				// a material is a shader AND its associated uniforms
				renderer.draw(va, ib, shader);

			}

			// sending r in as red to animate the color
			// this should actually be done before the draw, but whatevs
			//shader.setUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
			// change r to animate the color of the object
// 			if (r > 1.0)
// 				increment = -0.01f;
// 			else if (r < 0.0f)
// 				increment = 0.01f;
//			r += increment;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	} // end of scope

	glfwTerminate();
	return 0;
}