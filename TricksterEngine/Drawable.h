#pragma once
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

//Very high level interface class, parent of Drawable2D and Drawable3D
//Used to interface with OpenGL
//
//
//
//TODO: Make a manager class for each of these members so they are not owned by the drawable
namespace Trickster {
	class Drawable
	{
	public:
		Drawable();
		~Drawable();
		virtual void Draw() = 0;
	protected:
	};

}