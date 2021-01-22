#pragma once
#include "pch.h"
struct TextRenderInfo
{

};


struct Vertex
{
	float x, y, rotation, r, g, b, a;
};

struct FontInfo
{
	
};
struct Character {
	/*
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::vec2   Size;       // Size of glyph
    glm::vec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
    */
    float ax; // advance.x
    float ay; // advance.y

    float bw; // bitmap.width;
    float bh; // bitmap.rows;

    float bl; // bitmap_left;
    float bt; // bitmap_top;

    float tx; // x offset of glyph in texture coordinates
};
