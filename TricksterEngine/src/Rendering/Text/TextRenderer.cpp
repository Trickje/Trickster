#include "pch.h"
#include "Rendering/Text/TextRenderer.h"

#include "Events/EventManager.h"
using namespace Trickster;
/* Null, because instance will be initialized on demand. */
TextRenderer* TextRenderer::m_Instance = nullptr;
TextRenderer* Trickster::TextRenderer::Get()
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
	EventManager::GetInstance()->GameLoopEvents.OnRenderTransparent.AddListener(std::bind(&Trickster::TextRenderer::FlushQueue, this));
}

void Trickster::TextRenderer::RenderString(const std::string& a_String, glm::vec2 a_Position, const std::string& a_FontName)
{
	if(m_Fonts.find(a_FontName) == m_Fonts.end())
	{
		LoadFont(a_FontName);
	}
	m_RenderQueue.push_back(std::make_shared<TextRenderQueueObject>(a_String, a_Position, m_Fonts.at(a_FontName)));
//	m_Fonts.at(a_FontName)->Render(a_String, a_Position.x, a_Position.y, 1.f, {1.f,1.f,1.f});
}

void TextRenderer::FlushQueue()
{
	for(auto it : m_RenderQueue)
	{
		it->Font->Render(it->String, it->Position.x, it->Position.y,1.f, { 1.f, 1.f, 1.f });
	}
	m_RenderQueue.clear();
}

TextRenderer::TextRenderer()
{
}

Trickster::TextRenderer::~TextRenderer()
{
    delete m_Instance;
}

void Trickster::TextRenderer::LoadFont(std::string a_FontName)
{
	m_Fonts.insert(std::pair<std::string, std::shared_ptr<Font>>(a_FontName, std::make_shared<Font>()));
	m_Fonts.at(a_FontName)->LoadFromFile(a_FontName);
}
