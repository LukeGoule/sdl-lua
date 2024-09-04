#include "Uniforms.hpp"
#include "Engine.hpp"

Uniforms::Uniforms(Engine* instance) : EngineModule(instance) {
	this->m_vecUniforms = std::vector<Uniforms::Uniform_t*>{};
}

Uniforms::~Uniforms() {
	
}

void Uniforms::init() {

}

Uniforms* Uniforms::addUniform(Uniforms::Uniform_t* pUniform) {
	this->m_vecUniforms.push_back(pUniform);
	return this;
}

Uniforms* Uniforms::addUniform(Uniforms::UniformType type, void* pData, std::string name) {
	const auto pUniform = new Uniforms::Uniform_t{
		type, pData, name
	};
	
	this->m_vecUniforms.push_back(pUniform);

	return this;
}

void Uniforms::setOpenGLUniforms(GLuint iShaderProgram) {
	for (Uniforms::Uniform_t* pUniform : this->m_vecUniforms) {
		this->setSingleUniform(iShaderProgram, pUniform);
	}
}

void Uniforms::setSingleUniform(GLuint iShaderProgram, Uniforms::Uniform_t* pUniform) {
	GLuint iUniformLocation = glGetUniformLocation(iShaderProgram, pUniform->m_name.c_str());

	switch (pUniform->m_type) {
	case Uniforms::UniformType::FLOAT1:
		glUniform1fv(iUniformLocation, 1, (GLfloat*)pUniform->m_pData);
		break;

	case Uniforms::UniformType::INT1:
		glUniform1iv(iUniformLocation, 1, (GLint*)pUniform->m_pData);
		break;


	case Uniforms::UniformType::FLOAT2:
		glUniform2fv(iUniformLocation, 1, (GLfloat*)pUniform->m_pData);
		break;

	case Uniforms::UniformType::INT2:
		glUniform2iv(iUniformLocation, 1, (GLint*)pUniform->m_pData);
		break;

	case Uniforms::UniformType::MAT2:
		glUniformMatrix2fv(iUniformLocation, 1, GL_FALSE, (GLfloat*)pUniform->m_pData);
		break;


	case Uniforms::UniformType::FLOAT3:
		glUniform3fv(iUniformLocation, 1, (GLfloat*)pUniform->m_pData);
		break;

	case Uniforms::UniformType::INT3:
		glUniform3iv(iUniformLocation, 1, (GLint*)pUniform->m_pData);
		break;

	case Uniforms::UniformType::MAT3:
		glUniformMatrix3fv(iUniformLocation, 1, GL_FALSE, (GLfloat*)pUniform->m_pData);
		break;


	case Uniforms::UniformType::FLOAT4:
		glUniform4fv(iUniformLocation, 1, (GLfloat*)pUniform->m_pData);
		break;

	case Uniforms::UniformType::INT4:
		glUniform4iv(iUniformLocation, 1, (GLint*)pUniform->m_pData);
		break;

	case Uniforms::UniformType::MAT4:
		glUniformMatrix4fv(iUniformLocation, 1, GL_FALSE, (GLfloat*)pUniform->m_pData);
		break;

	default:
		// .. Do nothing.
		break;
	};
}