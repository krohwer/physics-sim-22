#include "pch.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "GuiUtils.h"

#include <GLFW/glfw3.h> // also uses glew 2.1.0 in precompiled header
#include <glm/glm.hpp> // glm version 9.9.8
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"
#include "Camera.h"

#include "Input.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#include "Environment.h"
#include "Shape.h"
#include "PhysicsObject.h"
#include "Collision.h"
#include "Manifold.h"
#include "PhysicsMath.h"

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

		// CREATE CAMERA //
		float halfWidth = (float)windowWidth;
		float halfHeight = (float)windowHeight;

		Camera camera(-halfWidth, halfWidth, -halfHeight, halfHeight);

		// SET INPUT //
		cameraInput = &camera;
		glfwSetKeyCallback(window, keyCallback);
		glfwSetScrollCallback(window, scrollCallback);

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
		bool doPhysics = false;
		bool isPaused = false;

		// timing variables
		float accumulator = 0;
		float frameStart;
		float startTime = glfwGetTime();

		// Physics environment setup
		Environment env = Environment(2000.0f, 1000.0f, GRAVITY, timestep);
		// Pixel ratio hardcoded right now, very dependent on window size
		env.pixelRatio = 100.0f;

		// create walls
		Shape leftWallShape;
		Body leftWall(&leftWallShape, -0.5f, (0.5f * env.height));
		leftWall.mass = 0.0f;
		leftWall.scale = glm::vec3(1.0f, env.height, 1.0f);
// 		Shape rightWallShape;
// 		Body rightWall(&rightWallShape, env.width + 0.5f, (0.5f * env.height));
// 		rightWall.mass = 0.0f;
// 		rightWall.scale = glm::vec3(1.0f, 32.0f, 1.0f);
		Shape floorShape;
		Body floor(&floorShape, (0.5f * env.width), -0.5);
		floor.mass = 0.0f;
		floor.scale = glm::vec3(env.width, 1.0f, 1.0f);

		env.addBody(&floor);
		env.addBody(&leftWall);
		//env.addBody(&rightWall);

		// Vectors to store all object starting positions and velocities
		std::vector<glm::vec3> startPositions;
		std::vector<glm::vec3> startVelocities;

		// RENDER LOOP //

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */

			renderer.clear();

			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			std::cout << xpos << ", " << ypos << std::endl;

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
					env.step();
					accumulator -= timestep;
				}
			}

			{ // SCOPE TO CALCULATE MVP MATRIX AND DRAW AN OBJECT //

				camera.recalculateView();

				// render each object
				for (Body& body : env.bodyList) {
					// identity model matrix
					glm::mat4 model(1.0f);
					/* DO ANY MODEL MATRIX TRANSFORMATIONS */
					// translate, then rotate, then scale.  VERY IMPORTANT
					model = glm::translate(model, body.position * env.pixelRatio);
					model = glm::rotate(model, body.rotation, glm::vec3(0, 0, 1));
					model = glm::scale(model, body.scale);

					// multiply the model, view, and projection matrices in reverse order to create the mvp.  We're kinda ignoring the view matrix since we'll use a static camera
					glm::mat4 mvp = camera.projectionMatrix * camera.viewMatrix * model;

					// set the matrix uniform for the mvp matrix so it updates every frame
					shader.setUniformMat4f("u_MVP", mvp);

					// infinite mass is gray!
					if (body.mass == 0) {
						body.color.a = 0.4f;
						shader.setUniform4f("u_Color", body.color);
					}
					else {
						body.color.a = 1.0f;
						shader.setUniform4f("u_Color", body.color);
					}

					// call the renderer to draw something
					// send in a vertex array, an index buffer, and a shader
					// in a more traditional setup, we would be using a material instead of a shader
					// a material is a shader AND its associated uniforms
					renderer.draw(va, ib, shader);
				}

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

			// IMGUI WINDOWS //

			// Forces next window to be a width of 500px and height of 600px upon launch of application
			// Users can still resize it afterwards
			// TODO: throw the sizes to a variable outside of the render loop
			ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiCond_FirstUseEver);

			// TODO: Potentially throw all created windows into GuiUtils for structure
			// Window to manage environment objects
			ImGui::Begin("Control Panel");

			// Creates a new object at the center of the screen
			if (ImGui::Button("Create Object")) {
				if (!doPhysics && !isPaused) {
					// Cannot press this button if the simulation is running or paused
					Shape shape;
					Body object(&shape, (camera.cPosition.x / env.pixelRatio), (camera.cPosition.y / env.pixelRatio));
					env.addBody(&object);
				}
			}
			// Removes all objects in the current environment
			if (ImGui::Button("Delete All Objects")) {
				if (!doPhysics && !isPaused) {
					// Cannot press this button if the simulation is running or paused
					doPhysics = false;
					env.bodyList.clear();
					startPositions.clear();
					startVelocities.clear();
					env.addBody(&floor);
					env.addBody(&leftWall);
					//env.addBody(&rightWall);
				}
			}

			gui_utils::createAllObjectMenus(env);

			ImGui::End(); // End of Control Panel Window

			// Forces window to a width of 700px and height of 400px upon launch of application
			// Users can still resize it afterwards
			// TODO: throw the sizes to a variable outside of the render loop
			ImGui::SetNextWindowSize(ImVec2(700, 400), ImGuiCond_FirstUseEver);

			// Window with buttons to manage the set up experiment
			ImGui::Begin("Simulation Manager");

			// TODO: Mess with the style of the buttons when they're disabled
			// Begins simulation and stores initial objects
			if (ImGui::Button("Start Simulation")) {
				if (!doPhysics && !isPaused) {
					// Cannot press this button if the simulation is running or paused

					// Physics pre-calculations
					for (Body& body : env.bodyList) {
						// save object starting positions and velocities
						startPositions.push_back(body.position);
						startVelocities.push_back(body.velocity);
						// Incoming storage manager POG?

						// while we're looping objects, go ahead and recalculate some important values
						body.computeInverseMass();
						body.computeInertia();
						body.shape->scaleX(body.scale);
						body.shape->scaleY(body.scale);
						// calculate the force of gravitation for the object into a vector and apply it
						glm::vec3 gravity(0, body.mass * env.gravity, 0);
						body.force -= gravity;
					}
					env.generatePairs();

					// Physics happens here
					frameStart = glfwGetTime();
					startTime = glfwGetTime();
					// TODO: Generate a set of ALL pairs to use for generating manifolds and detecting collisions between objects
					doPhysics = true;
				}
			}

			// Will pause/resume simulation once it's been started
			if (ImGui::Button("Pause/Resume Simulation")) {
				// Cannot press this button if the simulation was not started
				if (doPhysics) {
					// Pauses simulation if it's running
					/*std::cout << "Time elapsed: " << glfwGetTime() - startTime << std::endl;*/
					doPhysics = false;
					isPaused = true;
				}
				else if (isPaused) {
					// Reset start time and continue the physics simulation
					frameStart = glfwGetTime();
					startTime = glfwGetTime();
					doPhysics = true;
					isPaused = false;
				}
			}

			// Stops and resets objects as they were at the start of the simulation
			if (ImGui::Button("Stop/Reset Simulation")) {
				if (doPhysics || isPaused) {
					/*std::cout << "Time elapsed: " << glfwGetTime() - startTime << std::endl;*/
					int count = 0;
					for (Body& body : env.bodyList) {
						body.position = startPositions[count];
						body.velocity = startVelocities[count];
						count++;

						// clear forces!
						body.force = glm::vec3(0.0f);
					}
					startPositions.clear();
					startVelocities.clear();
					doPhysics = false;
					isPaused = false;
				}
			}

			ImGui::End(); // End of Simulation Manager Window

			// End of all ImGui windows

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