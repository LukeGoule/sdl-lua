#include "Shaders.hpp"
#include "Engine.hpp"

Shaders::Shaders(Engine* instance) : EngineModule(instance) {

}

Shaders::~Shaders() {

}

void Shaders::init() {

}

GLuint Shaders::compileShader(GLenum type, const char* szCode) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &szCode, nullptr);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cerr << "Shader Compilation Failed: " << infoLog << std::endl; // todo
	}

	return shader;
}

GLuint Shaders::compileShaderProgram(const char* szVertexShader, const char* szFragmentShader) {
	GLuint vertexShader = this->compileShader(GL_VERTEX_SHADER, szVertexShader);
	GLuint fragmentShader = this->compileShader(GL_FRAGMENT_SHADER, szFragmentShader);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	GLint success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cerr << "Shader Program Linking Failed: " << infoLog << std::endl; // todo
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}
