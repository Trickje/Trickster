#pragma once

namespace Trickster
{
	
	class Label
	{
	public:
		Label();
		~Label();
		void SetBoundBox(float a_X, float a_Y, float a_Width, float a_Height);
		glm::vec4 GetBoundBox();
	private:

		//(X, Y, Width, Height)
		glm::vec4 m_BoundBox;
	};
	
}