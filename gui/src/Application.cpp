#include "pch.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Menu.h"

#include <GLFW/glfw3.h> // also uses glew 2.1.0 in precompiled header
#include <glm/glm.hpp> // glm version 9.9.8
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"
#include "Camera.h"
#include "Input.h"
#include "StorageManager.h"
#include "ExperimentManager.h"

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
		float halfWidth = (float)windowWidth / 2.0f;
		float halfHeight = (float)windowHeight / 2.0f;

		Camera camera(-halfWidth, halfWidth, -halfHeight, halfHeight);

		// SET INPUT CALLBACKS //
		cameraInput = &camera;
		glfwSetKeyCallback(window, keyCallback);
		glfwSetScrollCallback(window, scrollCallback);
		glfwSetMouseButtonCallback(window, mouseButtonCallback);

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

		// IMGUI WINDOW CREATION //
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");

		// Setup for ImGui Window variables
		bool doPhysics = false;
		bool beginPhysics = false;

		// timing variables
		float accumulator = 0;
		float frameStart;
		float startTime = (float)glfwGetTime();

		// Physics environment setup
		Environment env = Environment(2000.0f, 1000.0f, DEFAULT_GRAVITY, timestep);

		// Initialize the storage manager
		StorageManager storage;

		Menu menu = Menu(&env, &storage, &camera, &doPhysics, &beginPhysics, &frameStart, &startTime);

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

			if (doPhysics) {
				const float currentTime = (float)glfwGetTime();

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

				// render the axes
				shader.setUniform4f("u_Color", 0.3f, 0.3f, 0.3f, 1.0f);

				unsigned int lineIndices[] = {
					3, 2
				};
				IndexBuffer xb(lineIndices, 2);
				renderer.setMVP(shader, camera, env.xAxis);
				renderer.drawLine(va, xb, shader);
				lineIndices[0] = 1;
				lineIndices[1] = 2;
				IndexBuffer yb(lineIndices, 2);
				renderer.setMVP(shader, camera, env.yAxis);
				renderer.drawLine(va, yb, shader);

				// this works, but it bogs down the renderer real bad. I think we should consider batch rendering for this,
				// the loop is too slow
// 				glm::vec3 position(0.5f, 0.0f, 0.0f);
// 				while (position.x < env.width) {
// 					renderer.setLineMVP(shader, camera, position, 0.0f);
// 					renderer.drawLine(va, yb, shader);
// 					position.x++;
// 				}

				// object index for highlighting
				int i = 1;
				// render each object
				for (Body& body : env.bodyList) {

					// set the MVP for the object
					renderer.setMVP(shader, camera, body);

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
					
					// draw highlight
					if (i == menu.highlight) {
						shader.setUniform4f("u_Color", 1.0f, 0.7f, 0.0f, 1.0f);
						unsigned int outline[] = {
							0, 1, 1, 2, 2, 3, 3, 0
						};
						renderer.drawLine(va, IndexBuffer(outline, 8), shader);
					}
					i++;

					// draw velocity line
					if (body.vSpeed > 0) {
						shader.setUniform4f("u_Color", 0.0f, 0.7f, 0.1f, 1.0f);
						float velocityLine[4] = {};
						velocityLine[0] = 0.0f; velocityLine[1] = 0.0f;
						if (doPhysics) {
							velocityLine[2] = body.velocity.x * 10;
							velocityLine[3] = body.velocity.y * 10;
						}
						else {
							velocityLine[2] = (body.vSpeed * cos(toRadians * body.vDirection)) * 10;
							velocityLine[3] = (body.vSpeed * sin(toRadians * body.vDirection)) * 10;
						}
						unsigned int velocityLineIndices[] = {
							0, 1
						};
						VertexArray velocityLineArray;
						VertexBuffer velocityLineBuffer(velocityLine, 2 * 2 * sizeof(float));
						velocityLineArray.addBuffer(velocityLineBuffer, layout);

						renderer.setLineMVP(shader, camera, body.position);
						renderer.drawLine(velocityLineArray, IndexBuffer(velocityLineIndices, 2), shader);
					}

				}

			} // end of MVP matrix scope

			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			xpos -= halfWidth;
			ypos = halfHeight - ypos;

// 			if (!doPhysics && !isPaused && createObject) {
// 				// Cannot press this button if the simulation is running or paused
// 				Shape shape;
// 				Body object(&shape, (((xpos * camera.cZoom) + camera.cPosition.x) / env.pixelRatio), (((ypos * camera.cZoom) + camera.cPosition.y) / env.pixelRatio));
// 				env.addBody(&object);
// 				createObject = false;
// 			}

			// IMGUI WINDOWS //

			menu.createMenuBar();

			menu.createControlPanel();

			menu.cleanUp();

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