#include "pch.h"
#include "Rendering/Text/Internal/Font.h"


#include "Core/Application.h"
#include "Rendering/ShaderManager.h"
#include "Rendering/Shader.h"
#include "Rendering/Window.h"
#include <glm/gtc/type_ptr.hpp>
using namespace TE;


#ifdef TRICKSTER_OPENGL


void Font::Initialize()
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
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (const GLvoid*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (const GLvoid*)(5 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void Font::LoadFromFile(const std::string& a_FileName)
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
    FT_GlyphSlot g = m_Face->glyph;
    int w = 0;
    int h = 0;

    for (int i = 0; i < 128; i++) {
        if (FT_Load_Char(m_Face, i, FT_LOAD_RENDER)) {
            fprintf(stderr, "Loading character %c failed!\n", i);
            continue;
        }

        w += g->bitmap.width;
        h = max(h, static_cast<int>(g->bitmap.rows));
    }

    /* you might as well save this value as it is needed later on */
    atlas_width = static_cast<float>(w);
    atlas_height = static_cast<float>(h);


    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int x = 0;

    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(m_Face, c, FT_LOAD_RENDER))
            continue;
        m_Characters[c].ax = g->advance.x >> 6;
        m_Characters[c].ay = g->advance.y >> 6;

        m_Characters[c].bw = g->bitmap.width;
        m_Characters[c].bh = g->bitmap.rows;

        m_Characters[c].bl = g->bitmap_left;
        m_Characters[c].bt = g->bitmap_top;

        m_Characters[c].tx = static_cast<float>(x) / static_cast<float>(w);
    	
        glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

        x += g->bitmap.width;
    }

	

    
    
    glBindTexture(GL_TEXTURE_2D, 0);
    // destroy FreeType once we're finished
    FT_Done_Face(m_Face);
    FT_Done_FreeType(m_Library);
}

void Font::Render()
{
    Shader* shader = ShaderManager::GetShader("BasicFont");
    shader->Bind();
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(Application::Get()->GetWindow()->GetWidth()), 0.0f, static_cast<float>(Application::Get()->GetWindow()->GetHeight()));
    // activate corresponding render state
    glUniformMatrix4fv(shader->GetUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    
    glBufferData(GL_ARRAY_BUFFER, coords.size() * 9 * sizeof(float), &coords[0], GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, coords.size());
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    coords.clear();
}

bool Font::operator==(const Font& other)
{

    return m_FileName == other.m_FileName;
}

void Font::AddVertexData(std::string text, float x, float y, float scale, glm::vec4 color)
{
    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character* ch = &m_Characters[*c];

        float x2 = x + ch->bl * scale;
        float y2 = -y - ch->bt * scale;
        float w = ch->bw * scale;
        float h = ch->bh * scale;

        /* Advance the cursor to the start of the next character */
        x += ch->ax * scale;
        y += ch->ay * scale;
        float z = 0.f;


        coords.push_back({ x2,   -y2  , z, ch->tx,      0, color.r, color.g, color.b, color.a });
        coords.push_back({ x2 + w, -y2    ,  z,ch->tx + ch->bw / atlas_width,   0 , color.r, color.g, color.b, color.a });
        coords.push_back({ x2,     -y2 - h, z, ch->tx,   ch->bh / atlas_height, color.r, color.g, color.b, color.a }); //remember: each glyph occupies a different amount of vertical space
        coords.push_back({ x2 + w, -y2    , z,  ch->tx + ch->bw / atlas_width,   0 , color.r, color.g, color.b, color.a });
        coords.push_back({ x2,     -y2 - h, z,  ch->tx, ch->bh / atlas_height , color.r, color.g, color.b, color.a });
        coords.push_back({ x2 + w, -y2 - h, z, ch->tx + ch->bw / atlas_width,  ch->bh / atlas_height , color.r, color.g, color.b, color.a });
    }
}


#endif


#ifdef TRICKSTER_VULKAN


void Font::AddVertexData(std::string text, float x, float y, float scale, glm::vec4 color)
{
}

void Font::Initialize()
{
}


void Font::LoadFromFile(const std::string& a_FileName)
{

}

void Font::Render()
{
}

bool Font::operator==(const Font& other)
{
    return true;
}


#endif

Font::Font()
{
    Initialize();
}

Font::~Font()
{
}
