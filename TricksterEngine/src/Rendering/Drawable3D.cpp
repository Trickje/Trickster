#include "pch.h"
#include "Drawable3D.h"

#include <glm/ext/matrix_transform.inl>



#include "Camera.h"
#include "MeshManager.h"
#include "ShaderManager.h"
#include "TinyObjLoader.h"
#include "Shader.h"
#include "Texture.h"
#include "TextureManager.h"

namespace Trickster {
	//Don't use this constructor!!
	Drawable3D::Drawable3D()
		: m_ModelMatrix(glm::mat4(1.0)), m_RotationMatrix(glm::mat4(1.0f)), m_TranslationMatrix(glm::mat4(1.0f))
	{
		m_DrawData = std::make_shared<DrawData>();
		m_ShaderPath = "";
	}
	
	Drawable3D::Drawable3D(const std::string& a_ModelFileName, const glm::vec3& a_Position, const glm::vec3& a_Scale,
		const std::string& a_ShaderFileName)
		: m_ModelMatrix(glm::mat4(1.0)), m_RotationMatrix(glm::mat4(1.0f)), m_TranslationMatrix(glm::mat4(1.0f))
	{
		Initialize(a_ModelFileName, a_Position, a_Scale, a_ShaderFileName);
		
	}


	Drawable3D::~Drawable3D()
	{
		delete m_DrawData.get();
	}

	void Drawable3D::Initialize(const std::string & a_ModelFileName, const glm::vec3 & a_Position, const glm::vec3 & a_Scale, const std::string & a_ShaderFileName)
	{
		//Initializes data
		m_DrawData = std::make_shared<DrawData>();
		m_ModelMatrix = glm::mat4(1.0f);
		m_RotationMatrix = glm::mat4(1.0f);
		m_TranslationMatrix = glm::mat4(1.0f);
		m_Yaw = 0.f;
		m_Pitch = 0.f;
		m_Roll = 0.f;
		//Subscribes to be managed by the MeshManager
		MeshManager::GetInstance()->m_Drawable3Ds.push_back(std::shared_ptr<Drawable3D>(this));
		//Fills in data
		m_TextureBase = "Models/";
		LoadMesh(a_ModelFileName);
		SetPosition(a_Position);
		SetShaderPath(a_ShaderFileName);
		SetScale(a_Scale);
	}

	void Drawable3D::SetShaderPath(const std::string & a_FilePath)
	{
		m_ShaderPath = a_FilePath;
	}


	//This is handled by the engine. Don't manually draw it
	void Drawable3D::Draw(std::shared_ptr<Camera> a_Camera)
	{
		CalculateRotationMatrix();
		m_ModelMatrix = m_RotationMatrix * m_TranslationMatrix;
		ShaderManager::GetShader(m_ShaderPath)->Bind();
		const auto UniformLoc = glGetUniformLocation(ShaderManager::GetShader(m_ShaderPath)->Get(), "MVP");
		glm::mat4 MVP = a_Camera->GetProjection() * a_Camera->GetView() * m_ModelMatrix;
		glUniformMatrix4fv(UniformLoc, 1, GL_FALSE, &MVP[0][0]);

		TextureManager::GetTexture(std::string(m_TextureBase + m_TextureFile))->Bind();
		m_DrawData->vb->Bind();
		m_DrawData->va->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m_Vertices.size()));
	}

	void Drawable3D::LoadMesh(const std::string& a_FileName)
	{

		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string basedir = "Models/";
		std::string FilePath = basedir + a_FileName;

		
		std::string warn;
		std::string err;

		bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, FilePath.c_str(),basedir.c_str());

		if (!warn.empty()) {
			LOG_WARNING(warn);
		}

		if (!err.empty()) {
			LOG_ERROR(err);
		}

		if (!ret) {
		//	exit(1);
		}
#pragma warning(push)
#pragma warning(disable: 4101)
#pragma warning (disable : 26451)
#pragma warning (disable : 26495)
#pragma warning (disable : 26812)
#pragma warning (disable : 4834)
		// Loop over shapes
		for (size_t s = 0; s < (size_t)shapes.size(); s++) {
			// Loop over faces(polygon)
			size_t index_offset = 0;
			for (size_t f = 0; f < (size_t)shapes[s].mesh.num_face_vertices.size(); f++) {
				int fv = shapes[s].mesh.num_face_vertices[f];

				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
					tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
					tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
					tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
					tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
					tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
					tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
					tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];
					// Optional: vertex colors
					// tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
					// tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
					// tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];
					/*
					 Vertex* vertex = new Vertex();
					
					vertex->position.x = vx;
					vertex->position.y = vy;
					vertex->position.z = vz;

					vertex->color.r = red;
					vertex->color.g = green;
					vertex->color.b = blue;

					vertex->tex.x = tx;
					vertex->tex.y = ty;
					*/
					if(ty > 0.5f)
					{
						//LOG(ty);
					}
					m_Vertices.push_back({vx, vy,vz,nx,ny,nz,tx,ty});
				}
				index_offset += (size_t)fv;

				// per-face material
				shapes[s].mesh.material_ids[f];
			}
		}
#pragma warning(pop)
		MakeBuffers();
		for(int i = 0; i < materials.size(); i++)
		{
			m_TextureFile = materials[i].diffuse_texname;
			TextureManager::GetTexture(m_TextureBase + m_TextureFile);
		}

	}

	void Drawable3D::SetPosition(const glm::vec3& a_Position)
	{
		m_TranslationMatrix[3][0] = a_Position.x;
		m_TranslationMatrix[3][1] = a_Position.y;
		m_TranslationMatrix[3][2] = a_Position.z;
	}

	glm::vec3 Drawable3D::GetPosition() const
	{
		return glm::vec3(m_TranslationMatrix[3][0], m_TranslationMatrix[3][1], m_TranslationMatrix[3][2]);
	}

	void Drawable3D::Move(const glm::vec3& a_Offset)
	{
		m_TranslationMatrix[3][0] += a_Offset.x;
		m_TranslationMatrix[3][1] += a_Offset.y;
		m_TranslationMatrix[3][2] += a_Offset.z;
	}

	glm::vec3 Drawable3D::GetForward() const
	{
		return glm::vec3(m_ModelMatrix[2][0],m_ModelMatrix[2][1], m_ModelMatrix[2][2]);
	}

	glm::vec3 Drawable3D::GetRight() const
	{
		return glm::vec3(m_ModelMatrix[0][0], m_ModelMatrix[0][1], m_ModelMatrix[0][2]);
	}

	glm::vec3 Drawable3D::GetUp() const
	{
		return glm::vec3(m_ModelMatrix[1][0], m_ModelMatrix[1][1], m_ModelMatrix[1][2]);
	}

	void Drawable3D::LookAt(const glm::vec3 & target, const glm::vec3 & tmp)
	{
		const glm::vec3 forward = glm::normalize(GetPosition() - target);
		const glm::vec3 right = glm::cross(glm::normalize(tmp), forward);
		const glm::vec3 up = glm::cross(forward, right);
		const glm::vec3 pos(GetPosition());
		m_ModelMatrix = glm::mat4();
		m_ModelMatrix[0][0] = right.x;
		m_ModelMatrix[0][1] = right.y;
		m_ModelMatrix[0][2] = right.z;
		m_ModelMatrix[1][0] = up.x;
		m_ModelMatrix[1][1] = up.y;
		m_ModelMatrix[1][2] = up.z;
		m_ModelMatrix[2][0] = forward.x;
		m_ModelMatrix[2][1] = forward.y;
		m_ModelMatrix[2][2] = forward.z;

		m_ModelMatrix[3][0] = pos.x;
		m_ModelMatrix[3][1] = pos.y;
		m_ModelMatrix[3][2] = pos.z;
	}

	void Drawable3D::SetScale(const glm::vec3 & a_Scale)
	{
		m_ModelMatrix[0][3] = a_Scale.x;
		m_ModelMatrix[1][3] = a_Scale.y;
		m_ModelMatrix[2][3] = a_Scale.z;
	}

	void Drawable3D::Rotate(const float a_DeltaYaw, const float a_DeltaPitch, const float a_DeltaRoll)
	{
		//m_RotationMatrix = glm::rotate(m_RotationMatrix, glm::radians(a_DeltaYaw), GetUp());
		//m_RotationMatrix = glm::rotate(m_RotationMatrix, glm::radians(a_DeltaPitch), GetRight());
		//m_RotationMatrix = glm::rotate(m_RotationMatrix, glm::radians(a_DeltaRoll), GetForward());
		
		m_Yaw += a_DeltaYaw;
		m_Pitch += a_DeltaPitch;
		m_Roll += a_DeltaRoll;
		
	}

	void Drawable3D::CalculateRotationMatrix()
	{
		m_RotationMatrix = glm::identity<glm::mat4>();
		m_RotationMatrix = glm::rotate(m_RotationMatrix, glm::radians(m_Yaw), glm::vec3(0.f,1.f,0.f));
		m_RotationMatrix = glm::rotate(m_RotationMatrix, glm::radians(m_Pitch), glm::vec3(1.f, 0.f, 0.f));
		m_RotationMatrix = glm::rotate(m_RotationMatrix, glm::radians(m_Roll), glm::vec3(0.f, 0.f, 1.f));
	}

	void Drawable3D::MakeBuffers()
	{
		m_DrawData->vb =new VertexBuffer(&m_Vertices[0], (unsigned int)m_Vertices.size() * 8 * sizeof(float));
		m_DrawData->layout = new VertexBufferLayout();
		m_DrawData->layout->Push<float>(3);
		m_DrawData->layout->Push<float>(3, true);
		m_DrawData->layout->Push<float>(2);
		m_DrawData->va = new VertexArray();
		m_DrawData->va->AddBuffer(*m_DrawData->vb, *m_DrawData->layout);
		
	}
}
