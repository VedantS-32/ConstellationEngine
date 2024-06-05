#include "CStellpch.h"
#include "OpenGLMaterial.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace CStell
{
	OpenGLMaterial::OpenGLMaterial(const std::string& shaderPath)
	{
		m_Shader = Shader::Create(shaderPath);
		m_UniformBuffer = UniformBuffer::Create(1);
		m_UniformBuffer->Bind();
		m_UniformBuffer->SetBufferSize(128);

		ExtractShaderUniform("ModelProps");
		m_UniformBuffer->Unbind();
	}

	void OpenGLMaterial::RecompileShaders()
	{
		m_Shader->RecompileShaders();
		ExtractShaderUniform("ModelProps");
		UpdateShaderUniform("ModelProps");
	}

	void OpenGLMaterial::ExtractShaderUniform(std::string uniformBlockName)
	{
		ShaderDataType GLType;
		uint32_t shaderID = m_Shader->GetRendererID();
		GLint blockIndex = m_Shader->GetUniformBlockIndex(uniformBlockName);

		GLint blockSize;
		glGetActiveUniformBlockiv(shaderID, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

		GLint numUniforms;
		glGetActiveUniformBlockiv(shaderID, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &numUniforms);

		GLuint* uniformIndices = new GLuint[numUniforms];
		glGetActiveUniformBlockiv(shaderID, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, (GLint*)uniformIndices);

		GLint* uniformOffsets = new GLint[numUniforms];
		glGetActiveUniformsiv(shaderID, numUniforms, uniformIndices, GL_UNIFORM_OFFSET, uniformOffsets);

		GLint* uniformSizes = new GLint[numUniforms];
		glGetActiveUniformsiv(shaderID, numUniforms, uniformIndices, GL_UNIFORM_SIZE, uniformSizes);
		
		for (int i = 0; i < numUniforms; ++i) {

			GLint uniformOffset = uniformOffsets[i];
			GLint uniformSize = uniformSizes[i];
			GLint uniformIndex = uniformIndices[i];
			GLenum uniformType;
			GLchar uniformName[256]; // Adjust buffer size as needed
			glGetActiveUniform(shaderID, uniformIndex, sizeof(uniformName), nullptr, &uniformSize, &uniformType, uniformName);

			switch (uniformType)
			{
			case GL_INT:
				GLType = ShaderDataType::Int;
				if (m_IntUniforms[uniformName] = (int)nullptr)
					m_IntUniforms[uniformName] = 0;
				break;
			case GL_INT_VEC2:
				GLType = ShaderDataType::Int2;
				if (m_IntUniforms[uniformName] = (int)nullptr)
					m_Int2Uniforms[uniformName] = glm::uvec2{ 0 };
				break;
			case GL_INT_VEC3:
				GLType = ShaderDataType::Int3;
				if (m_IntUniforms[uniformName] = (int)nullptr)
					m_Int3Uniforms[uniformName] = glm::uvec3{ 0 };
				break;
			case GL_INT_VEC4:
				GLType = ShaderDataType::Int4;
				if (m_IntUniforms[uniformName] = (int)nullptr)
					m_Int4Uniforms[uniformName] = glm::uvec4{ 0 };
				break;
			case GL_FLOAT:
				GLType = ShaderDataType::Float;
				if (m_IntUniforms[uniformName] = (int)nullptr)
					m_FloatUniforms[uniformName] = 0.0f;
				break;
			case GL_FLOAT_VEC2:
				GLType = ShaderDataType::Float2;
				if (m_IntUniforms[uniformName] = (int)nullptr)
					m_Float2Uniforms[uniformName] = glm::vec2{ 0.0f };
				break;
			case GL_FLOAT_VEC3:
				GLType = ShaderDataType::Float3;
				if (m_IntUniforms[uniformName] = (int)nullptr)
					m_Float3Uniforms[uniformName] = glm::vec3{ 0.0f };
				break;
			case GL_FLOAT_VEC4:
				GLType = ShaderDataType::Float4;
				if (m_IntUniforms[uniformName] = (int)nullptr)
					m_Float4Uniforms[uniformName] = glm::vec4{ 0.0f };
				break;
			case GL_FLOAT_MAT3:
				GLType = ShaderDataType::Mat3;
				if (m_IntUniforms[uniformName] = (int)nullptr)
					m_Mat3Uniforms[uniformName] = glm::mat3{ 0.0f };
				break;
			case GL_FLOAT_MAT4:
				GLType = ShaderDataType::Mat4;
				if (m_IntUniforms[uniformName] = (int)nullptr)
					m_Mat4Uniforms[uniformName] = glm::mat4{ 0.0f };
				break;
			case GL_SAMPLER_2D:
				GLType = ShaderDataType::Int;
				break;
			default:
				GLType = ShaderDataType::None;
				CSTELL_CORE_ERROR("Unknown Shader Datatype Uniform Name : {0}, GLenum : {1}", uniformName, (uint32_t)uniformType);
				break;
			}

			m_Uniforms[uniformName] = GLType;


			//CSTELL_CORE_INFO("Uniform type: {0}, name: {1}, size: {2}, offset: {3}", uniformType, uniformName, uniformSize, uniformOffset);
		}

		delete[] uniformIndices;
		delete[] uniformOffsets;
		delete[] uniformSizes;
	}

	void OpenGLMaterial::UpdateShaderUniform(std::string uniformBlockName)
	{
		m_UniformBuffer->Bind();

		void* buffer = glMapBuffer(GL_UNIFORM_BUFFER, GL_READ_WRITE);
		uint32_t offset = 0;

		uint32_t shaderID = m_Shader->GetRendererID();
		GLint blockIndex = m_Shader->GetUniformBlockIndex(uniformBlockName);

		GLint blockSize;
		glGetActiveUniformBlockiv(shaderID, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

		GLint numUniforms;
		glGetActiveUniformBlockiv(shaderID, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &numUniforms);

		GLuint* uniformIndices = new GLuint[numUniforms];
		glGetActiveUniformBlockiv(shaderID, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, (GLint*)uniformIndices);

		GLint* uniformOffsets = new GLint[numUniforms];
		glGetActiveUniformsiv(shaderID, numUniforms, uniformIndices, GL_UNIFORM_OFFSET, uniformOffsets);

		GLint* uniformSizes = new GLint[numUniforms];
		glGetActiveUniformsiv(shaderID, numUniforms, uniformIndices, GL_UNIFORM_SIZE, uniformSizes);

		for (int i = 0; i < numUniforms; ++i) {

			GLint uniformOffset = uniformOffsets[i];
			GLint uniformSize = uniformSizes[i];
			GLint uniformIndex = uniformIndices[i];
			GLenum uniformType;
			GLchar uniformName[256]; // Adjust buffer size as needed
			glGetActiveUniform(shaderID, uniformIndex, sizeof(uniformName), nullptr, &uniformSize, &uniformType, uniformName);

			offset = uniformOffset;

			switch (uniformType)
			{
			case GL_INT:
				memcpy(static_cast<char*>(buffer) + offset, &m_IntUniforms[uniformName], sizeof(int));
				break;
			case GL_INT_VEC2:
				memcpy(static_cast<char*>(buffer) + offset, &m_Int2Uniforms[uniformName], sizeof(int) * 2);
				break;
			case GL_INT_VEC3:
				memcpy(static_cast<char*>(buffer) + offset, &m_Int3Uniforms[uniformName], sizeof(int) * 3);
				break;
			case GL_INT_VEC4:
				memcpy(static_cast<char*>(buffer) + offset, &m_Int4Uniforms[uniformName], sizeof(int) * 4);
				break;
			case GL_FLOAT:
				memcpy(static_cast<char*>(buffer) + offset, &m_FloatUniforms[uniformName], sizeof(float));
				break;
			case GL_FLOAT_VEC2:
				memcpy(static_cast<char*>(buffer) + offset, &m_Float2Uniforms[uniformName], sizeof(float) * 2);
				break;
			case GL_FLOAT_VEC3:
				memcpy(static_cast<char*>(buffer) + offset, &m_Float3Uniforms[uniformName], sizeof(float) * 3);
				break;
			case GL_FLOAT_VEC4:
				memcpy(static_cast<char*>(buffer) + offset, &m_Float4Uniforms[uniformName], sizeof(float) * 4);
				break;
			case GL_FLOAT_MAT3:
				break;
			case GL_FLOAT_MAT4:
				memcpy(static_cast<char*>(buffer) + offset, &m_Mat4Uniforms[uniformName], sizeof(glm::mat4));
				break;
			case GL_SAMPLER_2D:
				memcpy(static_cast<char*>(buffer) + offset, &m_IntUniforms[uniformName], sizeof(float));
				break;
			default:
				CSTELL_CORE_ERROR("Unknown Shader Datatype Uniform Name : {0}, GLenum : {1}", uniformName, (uint32_t)uniformType);
				break;
			}

			//CSTELL_CORE_INFO("Updated Uniform type: {0}, name: {1}, size: {2}, offset: {3}", uniformType, uniformName, uniformSize, offset);

			m_UniformBuffer->SetData(buffer, sizeof(buffer));
		}

		delete[] uniformIndices;
		delete[] uniformOffsets;
		delete[] uniformSizes;

		buffer = 0;

		glUnmapBuffer(GL_UNIFORM_BUFFER);
		m_UniformBuffer->Unbind();
	}
}