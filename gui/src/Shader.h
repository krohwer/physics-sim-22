#pragma once

class Shader {
	/// Filepath of the vertex shader.
	std::string m_FilePathVertex;
	/// Filepath of the fragment shader.
	std::string m_FilePathFragment;
	/// Caching system for uniforms. Prevents finding the location every time.
	std::unordered_map<std::string, int> m_UniformLocationCache;
	unsigned int m_RendererID;

public:
	Shader(const std::string& filepathVertex, const std::string& filepathFragment);
	~Shader();

	void bind() const;
	void unbind() const;

	// set uniforms
	// could write a templated setUniform function to set it, or parse the source code to do this automatically
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
private:
	/**
	 * Read the shader file and convert it into a string.
	 */
	std::string parseShader(const std::string& filepath);
	/**
	 * Tells OpenGL to compile our shaders.
	 */
	unsigned int compileShader(unsigned int type, const std::string& source);
	/**
	 * Provides OpenGL with our shader source code.
	 */
	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
	int getUniformLocation(const std::string& name);
};