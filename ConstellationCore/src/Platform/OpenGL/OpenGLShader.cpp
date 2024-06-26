#include "CStellpch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace CStell
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "Pixel")
			return GL_FRAGMENT_SHADER;

		CSTELL_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
		: m_ShaderPath(filepath)
	{
		PrepareShader();
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_Name(name)
	{
		CSTELL_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;

		shaderSources[GL_VERTEX_SHADER] = vertexSrc;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(shaderSources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	uint32_t OpenGLShader::GetUniformBlockIndex(const std::string& blockName)
	{
		return glGetUniformBlockIndex(m_RendererID, blockName.c_str());
	}

	void OpenGLShader::Set1i(const std::string& name, int value)
	{
		UploadUniform1i(name, value);
	}

	void OpenGLShader::Set2i(const std::string& name, glm::uvec2 value)
	{
		UploadUniform2i(name, value);
	}

	void OpenGLShader::Set3i(const std::string& name, glm::uvec3 value)
	{
		UploadUniform3i(name, value);
	}

	void OpenGLShader::Set4i(const std::string& name, glm::uvec4 value)
	{
		UploadUniform4i(name, value);
	}

	void OpenGLShader::Set1iArray(const std::string& name, int* value, uint32_t count)
	{
		UploadUniform1iArray(name, value, count);
	}

	void OpenGLShader::Set1f(const std::string& name, float value)
	{
		UploadUniform1f(name, value);
	}

	void OpenGLShader::Set2f(const std::string& name, glm::vec2 value)
	{
		UploadUniform2f(name, value);
	}

	void OpenGLShader::Set3f(const std::string& name, glm::vec3 value)
	{
		UploadUniform3f(name, value);
	}

	void OpenGLShader::Set4f(const std::string& name, glm::vec4 value)
	{
		UploadUniform4f(name, value);
	}

	void OpenGLShader::SetMat3f(const std::string& name, const glm::mat3& matrix)
	{
		UploadUniformMat3f(name, matrix);
	}

	void OpenGLShader::SetMat4f(const std::string& name, const glm::mat4& matrix)
	{
		UploadUniformMat4f(name, matrix);
	}

	void OpenGLShader::RecompileShaders()
	{
		PrepareShader();
	}

	void OpenGLShader::PrepareShader()
	{
		CSTELL_PROFILE_FUNCTION();

		std::string source = ParseShader(m_ShaderPath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		// Extract name from filepath
		auto lastSlash = m_ShaderPath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = m_ShaderPath.rfind('.');
		auto count = lastDot == std::string::npos ? m_ShaderPath.size() - lastSlash : lastDot - lastSlash;
		m_Name = m_ShaderPath.substr(lastSlash, count);
	}

	std::string OpenGLShader::ParseShader(std::string filepath)
	{
		CSTELL_PROFILE_FUNCTION();

		std::string content;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in) {
			in.seekg(0, std::ios::end);
			content.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&content[0], content.size());
			in.close();
		}
		else
		{
			CSTELL_CORE_ERROR("Could not open file '{0}'", filepath);
		}

		return content;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		CSTELL_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "//#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			CSTELL_CORE_ASSERT(eol != std::string::npos, "Syntax Error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			CSTELL_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(std::unordered_map<GLenum, std::string> shaderSources)
	{
		CSTELL_PROFILE_FUNCTION();

		GLuint program = glCreateProgram();
		CSTELL_CORE_ASSERT(shaderSources.size() <= 2, "Only two shaders are supported for now");
		std::array<GLenum, 2> glShaderIDs{};
		int glShaderIDIndex = 0;

		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				CSTELL_CORE_ERROR("{0}", infoLog.data());
				CSTELL_CORE_ASSERT(false, "Vertex shader compilation failure!");

				break;
			}

			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);

			for (auto id : glShaderIDs)
				glDeleteShader(id);

			CSTELL_CORE_ERROR("{0}", infoLog.data());
			CSTELL_CORE_ASSERT(false, "OpenGLShader link failure!");

			return;
		}

		for (auto id : glShaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		m_RendererID = program;
	}

	void OpenGLShader::UploadUniform1i(const std::string& name, int value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void OpenGLShader::UploadUniform2i(const std::string& name, glm::uvec2 value)
	{
		glUniform2i(GetUniformLocation(name), value.x, value.y);
	}

	void OpenGLShader::UploadUniform3i(const std::string& name, glm::uvec3 value)
	{
		glUniform3i(GetUniformLocation(name), value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniform4i(const std::string& name, glm::uvec4 value)
	{
		glUniform4i(GetUniformLocation(name), value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniform1iArray(const std::string& name, int* value, uint32_t count)
	{
		glUniform1iv(GetUniformLocation(name), count, value);
	}

	void OpenGLShader::UploadUniform1f(const std::string& name, float value)
	{
		glUniform1f(GetUniformLocation(name), value);
	}

	void OpenGLShader::UploadUniform2f(const std::string& name, glm::vec2 value)
	{
		glUniform2f(GetUniformLocation(name), value.x, value.y);
	}

	void OpenGLShader::UploadUniform3f(const std::string& name, glm::vec3 value)
	{
		glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniform4f(const std::string& name, glm::vec4 value)
	{
		glUniform4f(GetUniformLocation(name), value.r, value.g, value.b, value.a);
	}

	void OpenGLShader::UploadUniformMat3f(const std::string& name, const glm::mat3& matrix)
	{
		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4f(const std::string& name, const glm::mat4& matrix)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	int OpenGLShader::GetUniformLocation(const std::string& name)
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];

		int location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location == -1)
			CSTELL_CORE_WARN("Warning uniform {0} doesn't exist!", name);
		m_UniformLocationCache[name] = location;
		return location;
	}
}