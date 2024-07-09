#pragma once

#include "CStell/Renderer/ShaderType.h"
#include "CStell/Renderer/Material.h"

namespace CStell
{
	class OpenGLMaterial : public Material
	{
	public:
		OpenGLMaterial(const std::string& materialPath);
		~OpenGLMaterial() = default;

		virtual void RecompileShaders() override;
		virtual void ExtractShaderUniform(std::string uniformBlockName) override;
		virtual void UpdateShaderUniform(std::string UBOName) override;
	};
}