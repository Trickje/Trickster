#include "pch.h"
#include "IronMan.h"



IronMan::IronMan(const std::string & a_ModelFileName, const glm::vec3 & a_Position, const glm::vec3 & a_Scale, const std::string & a_ShaderFileName)
{
	if(a_ShaderFileName.empty())
	{
		Initialize(a_ModelFileName, a_Position, a_Scale);
	}else
	{
		Initialize(a_ModelFileName, a_Position, a_Scale, a_ShaderFileName);
	}
}


IronMan::~IronMan()
{
}
