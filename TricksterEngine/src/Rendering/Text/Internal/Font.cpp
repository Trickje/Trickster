#include "pch.h"
#include "Rendering/Text/Internal/Font.h"


#include "Core/Application.h"
#include "Rendering/ShaderManager.h"
#include "Rendering/Shader.h"
#include "Rendering/Window.h"
#include <glm/gtc/type_ptr.hpp>
using namespace Trickster;


#ifdef TRICKSTER_OPENGL


void Trickster::Font::Initialize()
{

    // configure VAO/VBO for texture quads
        // -----------------------------------
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	/*
	 * The size of each vertex
	 * Position: x, y, z = 3
	 * Color: r, g, b, a = 4
	 * TODO: implement batch rendering
	 * 
	 */
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void Trickster::Font::LoadFromFile(const std::string& a_FileName)
{

    FT_Error m_Error;
    FT_Library m_Library;
    FT_Face m_Face;
    if (FT_Init_FreeType(&m_Library))
    {
        LOG_ERROR("[Font] " + a_FileName + " Could not init FreeType Library");
        return;
    }
    if (FT_New_Face(m_Library, ("Resources/Fonts/" + a_FileName + ".ttf").c_str(), 0, &m_Face)) {
        LOG_ERROR("[Font] " + a_FileName + " Failed to load font");
        return;
}

    // set size to load glyphs as
    FT_Set_Pixel_Sizes(m_Face, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        if (FT_Load_Char(m_Face, c, FT_LOAD_RENDER))
        {
            LOG_ERROR("[Font] Failed to load Glyph");
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            m_Face->glyph->bitmap.width,
            m_Face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            m_Face->glyph->bitmap.buffer
            );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture,
            glm::vec2(m_Face->glyph->bitmap.width, m_Face->glyph->bitmap.rows),
            glm::vec2(m_Face->glyph->bitmap_left, m_Face->glyph->bitmap_top),
            m_Face->glyph->advance.x
        };
        m_Characters.insert(std::pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    // destroy FreeType once we're finished
    FT_Done_Face(m_Face);
    FT_Done_FreeType(m_Library);
}

void Trickster::Font::Render(std::string text, float x, float y, float scale, glm::vec3 color)
{
    Shader* shader = ShaderManager::GetShader("BasicFont");
    shader->Bind();
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(Application::Get()->GetWindow()->GetWidth()), 0.0f, static_cast<float>(Application::Get()->GetWindow()->GetHeight()));
    // activate corresponding render state
    glUniformMatrix4fv(shader->GetUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(shader->GetUniformLocation("textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(m_VAO);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = m_Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


#endif


#ifdef TRICKSTER_VULKAN



void Trickster::Font::Initialize()
{
}


void Trickster::Font::LoadFromFile(const std::string& a_FileName)
{

}


#endif

Trickster::Font::Font()
{
    Initialize();
}

Trickster::Font::~Font()
{
}
