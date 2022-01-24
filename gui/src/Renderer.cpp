#include "pch.h" // uses glew 2.1.0 in precompiled header

#include "Renderer.h"

#include "PhysicsMath.h"
#include <glm/gtc/matrix_transform.hpp>

Renderer::Renderer() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::clear() const {

	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::setMVP(Shader& shader, const Camera& camera, const Body& body) {
	// identity model matrix
	glm::mat4 model(1.0f);
	/* DO ANY MODEL MATRIX TRANSFORMATIONS */
	// translate, then rotate, then scale.  VERY IMPORTANT
	model = glm::translate(model, body.position * PIXEL_RATIO);
	model = glm::rotate(model, body.rotation, glm::vec3(0, 0, 1));
	model = glm::scale(model, body.scale);

	// multiply the model, view, and projection matrices in reverse order to create the mvp.  We're kinda ignoring the view matrix since we'll use a static camera
	glm::mat4 mvp = camera.projectionMatrix * camera.viewMatrix * model;

	// set the mvp uniform
	shader.setUniformMat4f("u_MVP", mvp);
}

void Renderer::setLineMVP(Shader& shader, const Camera& camera, const Body& body) {
	// identity model matrix
	glm::mat4 model(1.0f);
	/* DO ANY MODEL MATRIX TRANSFORMATIONS */
	// translate, then rotate, then scale.  VERY IMPORTANT
	model = glm::translate(model, body.position * PIXEL_RATIO);
	model = glm::rotate(model, body.rotation, glm::vec3(0, 0, 1));
	model = glm::scale(model, body.scale);

	// multiply the model, view, and projection matrices in reverse order to create the mvp.  We're kinda ignoring the view matrix since we'll use a static camera
	glm::mat4 mvp = camera.projectionMatrix * camera.viewMatrix * model;

	// set the mvp uniform
	shader.setUniformMat4f("u_MVP", mvp);
}

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {

	// bind the shader
	shader.bind();
	// before drawing, bind the vertex array, and the index buffer
	// using a VAO in this way is helpful for rendering multiple objects
	va.bind();
	ib.bind();

	// drawing in triangles for now, then the number of INDICES (not vertices), and the data type of the indices.
	glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::drawLine(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {

	// bind the shader
	shader.bind();
	// before drawing, bind the vertex array, and the index buffer
	// using a VAO in this way is helpful for rendering multiple objects
	va.bind();
	ib.bind();

	glLineWidth(5.0f);
	// drawing in triangles for now, then the number of INDICES (not vertices), and the data type of the indices.
	glDrawElements(GL_LINES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}