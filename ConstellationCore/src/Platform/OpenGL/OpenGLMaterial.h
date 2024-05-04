#pragma once

#include "CStell/Renderer/ShaderType.h"
#include "CStell/Renderer/Material.h"

namespace CStell
{
	class OpenGLMaterial : public Material
	{
	public:
		OpenGLMaterial(const std::string& shaderPath);
		~OpenGLMaterial() = default;

		virtual void RecompileShaders() override;
		virtual void ExtractShaderUniform() override;
		virtual void UpdateShaderUniform() override;
	};
}