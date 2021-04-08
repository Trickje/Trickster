#pragma once
#include "RenderAPI.h"

namespace Trickster
{
	class OpenGL : public RenderAPI
	{
	public:
		TRICKSTER_API void Initialize() override;
		TRICKSTER_API virtual ~OpenGL() {};

		TRICKSTER_API OpenGL();
		TRICKSTER_API virtual void DrawFrame() override;
		TRICKSTER_API virtual void Resize(int width, int height) override;
		TRICKSTER_API virtual void LoadModel(const std::string& a_ModelName) override;
		TRICKSTER_API virtual void DrawModel(const std::string& a_ModelName, const glm::mat4& a_ModelMatrix) override;
	};
}
