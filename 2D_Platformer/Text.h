#pragma once

#include <map>
#include <iostream>

#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "ShaderProgram.h"

struct Character {
	GLuint     TextureID;  // ID handle of the glyph texture
	glm::ivec2 Size;       // Size of glyph
	glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
	GLuint     Advance;    // Offset to advance to next glyph
};

class Text {
public:
	// Constructor
	Text(unsigned int width, unsigned int height, const char* vrtxShaderPath, const char* frgmtShaderPath, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT);
	
	// Functions
	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);


private:
	// Members
	unsigned int vaoId, vboId;
	ShaderProgram shaderProgramId;
	std::map<GLchar, Character> Characters;

};