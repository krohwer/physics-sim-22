#include "pch.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <GLFW/glfw3.h> // also uses glew 2.1.0 in precompiled header
#include <glm/glm.hpp> // glm version 9.9.8
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#include "Environment.h"
#include "Physics.h"
#include "PhysicsObject.h"

void printOneVector(glm::vec3 vector) {
	std::cout << "{" << vector.x << ", " << vector.y << ", " << vector.z << "}" << std::endl;
}

void printVectors(PhysicsObject object) {
	std::cout << "Position:";
	printOneVector(object.position);
	std::cout << "Velocity:";
	printOneVector(object.velocity);
	std::cout << "Acceleration:";
	printOneVector(object.acceleration);
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
	int windowWidth = 1600;
	int windowHeight = 900;
	glm::vec2 windowCenter(windowWidth / 2.0f, windowHeight / 2.0f);
	window = glfwCreateWindow(windowWidth, windowHeight, "Kinetics Lab", NULL, NULL);
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
		shader.setUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

		// UNBIND EVERYTHING //

		// unbind the buffers, to test use of vertex array object
		va.unbind();
		vb.unbind();
		ib.unbind();
		shader.unbind();

		// create a renderer
		Renderer renderer;

		// variables to represent red and how much we want it to change each tick
// 		float r = 0.0f;
// 		float increment = 0.01f;

		// IMGUI WINDOW CREATION //
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");

		// Setup for ImGui Window variables
		glm::vec3 translation(windowCenter.x, windowCenter.y, 0); // translate to the center of the screen by default
		bool drawObject = false;
		bool doPhysics = false;

		// Physics timing variables
		const float physicsFPS = 60;
		const float timestep = 1 / physicsFPS;
		float accumulator = 0;
		float frameStart;
		float startTime;

		// Physics environment and object setup
		Environment env = {16.0f, 9.0f, 9.81f, 0.0f}; // change the order of width and height because height->width make mad
		env.pixelRatio = windowHeight / env.height;
		// fix the translation
		translation = translation / env.pixelRatio;
		PhysicsObject object;
		object.mass = 10.0f;
		object.position = translation;

		// RENDER LOOP //

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */

			renderer.clear();

			// IMGUI INITIALIZATION
			// Must be called before any other ImGui code
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			// bind the shader
			// in a perfect world, you do this right before you actually draw an object, and have a shader cache to make sure shaders are not bound multiple times
			shader.bind();

			if (doPhysics) {
				const float currentTime = glfwGetTime();

				accumulator += currentTime - frameStart;

				frameStart = currentTime;

				while (accumulator > timestep) {
					// do the physics
					physics::updateObject(object, env, timestep);
					//std::cout << "Accumulator: " << accumulator << std::endl;
					//std::cout << "Timestep: " << timestep << std::endl;
					accumulator -= timestep;
				}
			}

			{ // SCOPE TO CALCULATE MVP MATRIX AND DRAW AN OBJECT //

				// identity model matrix
				// can use it to move the object with glm::translate(model, glm::vec3(xOffset, yOffset, zOffset));
				// the vec3 you're sending in is basically the displacement vector, telling how much the object moves in each direction
				// for multiple objects, we will need a separate model matrix for each one since it converts from "object" coordinates to "world" coordinates
				glm::mat4 model(1.0f);
				/* DO ANY MODEL MATRIX TRANSFORMATIONS */
				// translate, then rotate, then scale.  VERY IMPORTANT
				model = glm::translate(model, object.position * env.pixelRatio);
				//model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0, 0, 1)); // rotate the square 45 degrees
				//model = glm::scale(model, glm::vec3(2, 2, 1)); // double the size of the square

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
				//	ImGui nonsense butting in here
				// if statement allows us to toggle whether the object is drawn or not
				// since we're only dealing with one object, this is the easiest way to go about it FOR NOW
				if (drawObject)
					renderer.draw(va, ib, shader);

			} // end of mvp matrix scope

			// sending r in as red to animate the color
			// this should actually be done before the draw, but whatevs
			//shader.setUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
			// change r to animate the color of the object
// 			if (r > 1.0)
// 				increment = -0.01f;
// 			else if (r < 0.0f)
// 				increment = 0.01f;
//			r += increment;

			// Text at the top of the ImGui window
			ImGui::Begin("Control Panel");

			// Buttons to create/clear the object
			// Resets the object to the "default" position and allows it to be drawn
			if (ImGui::Button("Create Object")) {
				object.position = translation;
				physics::resetObject(object);
				drawObject = true;
			}
			// Removes the ability for the object to be drawn
			if (ImGui::Button("Delete Object")) {
				drawObject = false;
				doPhysics = false;
			}

			// Sliders to mess with horizontal and vertical position(s) of the object
			ImGui::Text("Object Horizontal Position");
			ImGui::SliderFloat("X Pos", &object.position.x, 0.5f, env.width - 0.5f);
			ImGui::Text("Object Vertical Position");
			ImGui::SliderFloat("Y Pos", &object.position.y, 0.5f, env.height - 0.5f);
			ImGui::Text("Object Horizontal Velocity");
			ImGui::SliderFloat("X Vel", &object.velocity.x, -50.0f, 50.0f);
			ImGui::Text("Object Vertical Velocity");
			ImGui::SliderFloat("Y Vel", &object.velocity.y, -50.0f, 50.0f);

			// Buttons to actually conduct default experiment
			if (ImGui::Button("Play Simulation")) {
				// Physics happens here
				frameStart = glfwGetTime();
				startTime = glfwGetTime();
				physics::applyGravity(object, env);
				doPhysics = true;
			}
			if (ImGui::Button("Stop/Reset Simulation")) {
				object.position = translation;
				std::cout << "Time elapsed: " << glfwGetTime() - startTime << std::endl;
				physics::resetObject(object);
				doPhysics = false;
			}

			ImGui::End();
			// Marks end of this ImGui window

			// Must be included after the above set of code related to ImGUI
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	} // end of GL scope

	// Delete things related to ImGUI
	// Similar to other delete functions
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}