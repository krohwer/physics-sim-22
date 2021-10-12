#include "pch.h"

#include "Shader.h"

#include "GLErrorManager.h"

Shader::Shader(const std::string& filepathVertex, const std::string& filepathFragment) {
	m_FilePathVertex = filepathVertex;
	m_FilePathFragment = filepathFragment;
	m_RendererID = 0;

	std::string vertexSource = parseShader(filepathVertex);
	// 	std::cout << "VERTEX SHADER" << std::endl;
	// 	std::cout << vertexSource << std::endl;
	std::string fragmentSource = parseShader(filepathFragment);
	// 	std::cout << "FRAGMENT SHADER" << std::endl;
	// 	std::cout << fragmentSource << std::endl;

	m_RendererID = createShader(vertexSource, fragmentSource);
}

Shader::~Shader() {
	glDeleteProgram(m_RendererID);
}

std::string Shader::parseShader(const std::string& filepath) {

	std::ifstream stream(filepath);

	std::string line;
	std::stringstream shaderString;
	while (getline(stream, line)) {
		shaderString << line << '\n';
	}

	// return the shader as a string
	return shaderString.str();
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source) {

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

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader) {

	// unsigned int vs GLuint: same thing, but using raw c++ is just compatible with other APIs
	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

void Shader::bind() const {

	glUseProgram(m_RendererID);
}

void Shader::unbind() const {

	glUseProgram(0);
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {

	glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
}

int Shader::getUniformLocation(const std::string& name) {

	int location;

	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		location = m_UniformLocationCache[name];
	else
		location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location == -1)
			std::cout << "Warning: uniform '" << name << "' doesn't exist" << std::endl;
		m_UniformLocationCache[name] = location;

	return location;
}
