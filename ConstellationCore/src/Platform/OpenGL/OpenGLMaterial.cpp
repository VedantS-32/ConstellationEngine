#include "CStellpch.h"
#include "OpenGLMaterial.h"

#include <glad/glad.h>

namespace CStell
{
	OpenGLMaterial::OpenGLMaterial(const std::string& shaderPath)
	{
		m_Shader = Shader::Create(shaderPath);
		ExtractShaderUniform();
	}

	void OpenGLMaterial::RecompileShaders()
	{
		m_Shader->RecompileShaders();
		ExtractShaderUniform();
		UpdateShaderUniform();
	}

	void OpenGLMaterial::ExtractShaderUniform()
	{
		uint32_t shaderID = m_Shader->GetRendererID();
		GLint numUniforms;
		glGetProgramiv(shaderID, GL_ACTIVE_UNIFORMS, &numUniforms);
		for (int i = 0; i < numUniforms; ++i)
		{
			ShaderDataType GLType;
			std::string typeName;
			char uniformName[256];
			GLsizei length;
			GLint size;
			GLenum type;
			glGetActiveUniform(shaderID, i, sizeof(uniformName), &length, &size, &type, uniformName);
			GLint location = glGetUniformLocation(shaderID, uniformName);

			switch (type)
			{
			case GL_INT:
				GLType = ShaderDataType::Int;
				if (m_IntUniforms[uniformName] = (int)nullptr)
					m_IntUniforms[uniformName] = 0;
				break;
			case GL_INT_VEC2:
				GLType = ShaderDataType::Int2;
				if(m_IntUniforms[uniformName] = (int)nullptr)
					m_Int2Uniforms[uniformName] = glm::uvec2{ 0 };
				break;
			case GL_INT_VEC3:
				GLType = ShaderDataType::Int3;
				if(m_IntUniforms[uniformName] = (int)nullptr)
				m_Int3Uniforms[uniformName] = glm::uvec3{ 0 };
				break;
			case GL_INT_VEC4:
				GLType = ShaderDataType::Int4;
				if(m_IntUniforms[uniformName] = (int)nullptr)
				m_Int4Uniforms[uniformName] = glm::uvec4{ 0 };
				break;
			case GL_FLOAT:
				GLType = ShaderDataType::Float;
				if(m_IntUniforms[uniformName] = (int)nullptr)
				m_FloatUniforms[uniformName] = 0.0f;
				break;
			case GL_FLOAT_VEC2:
				GLType = ShaderDataType::Float2;
				if(m_IntUniforms[uniformName] = (int)nullptr)
				m_Float2Uniforms[uniformName] = glm::vec2{ 0.0f };
				break;
			case GL_FLOAT_VEC3:
				GLType = ShaderDataType::Float3;
				if(m_IntUniforms[uniformName] = (int)nullptr)
				m_Float3Uniforms[uniformName] = glm::vec3{ 0.0f };
				break;
			case GL_FLOAT_VEC4:
				GLType = ShaderDataType::Float4;
				if(m_IntUniforms[uniformName] = (int)nullptr)
				m_Float4Uniforms[uniformName] = glm::vec4{ 0.0f };
				break;
			case GL_FLOAT_MAT3:
				GLType = ShaderDataType::Mat3;
				break;
			case GL_FLOAT_MAT4:
				GLType = ShaderDataType::Mat4;
				break;
			case GL_SAMPLER_2D:
				GLType = ShaderDataType::Int;
				break;
			default:
				GLType = ShaderDataType::None;
				CSTELL_CORE_ERROR("Unknown Shader Datatype Uniform Name : {0}, GLenum : {1}", uniformName, (uint32_t)type);
				break;
			}

			m_Uniforms[uniformName] = GLType;

			CSTELL_CORE_TRACE("Uniform Name: {0}, Type: {1}, GLEnum: {2}", uniformName, typeName, (uint32_t)type);
		}
	}

	void OpenGLMaterial::UpdateShaderUniform()
	{
		for (auto& uniform : m_Uniforms)
		{
			std::string uniformName = uniform.first;
			switch (uniform.second)
			{
			case ShaderDataType::Int:
				m_Shader->Set1i(uniformName, m_IntUniforms[uniformName]);
				break;
			case ShaderDataType::Int2:
				m_Shader->Set2i(uniformName, m_Int2Uniforms[uniformName]);
				break;
			case ShaderDataType::Int3:
				m_Shader->Set3i(uniformName, m_Int3Uniforms[uniformName]);
				break;
			case ShaderDataType::Int4:
				m_Shader->Set4i(uniformName, m_Int4Uniforms[uniformName]);
				break;
			case ShaderDataType::Float:
				m_Shader->Set1f(uniformName, m_FloatUniforms[uniformName]);
				break;
			case ShaderDataType::Float2:
				m_Shader->Set2f(uniformName, m_Float2Uniforms[uniformName]);
				break;
			case ShaderDataType::Float3:
				m_Shader->Set3f(uniformName, m_Float3Uniforms[uniformName]);
				break;
			case ShaderDataType::Float4:
				m_Shader->Set4f(uniformName, m_Float4Uniforms[uniformName]);
				break;
			case ShaderDataType::Mat3:
				break;
			case ShaderDataType::Mat4:
				break;
			default:
				break;
			}
		}
	}
}
