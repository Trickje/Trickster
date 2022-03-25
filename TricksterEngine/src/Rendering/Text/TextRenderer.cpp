#include "pch.h"
#include "Rendering/Text/TextRenderer.h"

#include "Events/EventManager.h"
using namespace TE;
/* Null, because instance will be initialized on demand. */
TextRenderer* TextRenderer::m_Instance = nullptr;
TextRenderer* TextRenderer::Get()
{
	if(m_Instance == nullptr)
	{
        m_Instance = new TextRenderer();
        m_Instance->Initialize();
	}
	
    return m_Instance;
}

void TextRenderer::Initialize()
{
	EventManager::GetInstance()->GameLoopEvents.OnRenderTransparent.AddListener(std::bind(&TextRenderer::FlushQueue, this));
}

void TextRenderer::RenderString(const std::string& a_String, glm::vec2 a_Position, float a_Scale, glm::vec4 a_Color, const std::string& a_FontName)
{
	if(m_Fonts.find(a_FontName) == m_Fonts.end())
	{
		LoadFont(a_FontName);
	}
	std::shared_ptr<Font> font = m_Fonts.at(a_FontName);
	font->AddVertexData(a_String, a_Position.x, a_Position.y, a_Scale, a_Color);
	bool isNew = true;
	for(int i = 0; i < m_RenderQueue.size(); i++)
	{
		if (m_RenderQueue[i] == font)
			isNew = false;
	}
	if(isNew)
	{
		m_RenderQueue.push_back(font);
	}
}

void TextRenderer::FlushQueue()
{
	for(auto it : m_RenderQueue)
	{
		it->Render();
	}
	
	m_RenderQueue.clear();
}

TextRenderer::TextRenderer()
{
}

TextRenderer::~TextRenderer()
{
	//m_Fonts.clear();
}

void TextRenderer::LoadFont(std::string a_FontName)
{
	m_Fonts.insert(std::pair<std::string, std::shared_ptr<Font>>(a_FontName, std::make_shared<Font>()));
	m_Fonts.at(a_FontName)->LoadFromFile(a_FontName);
}
