#include "Font.h"
#include "ResourceManager.h"
#include "IO.h"
#include "Camera.h"

bool FontManager::loaded = false;
ShaderProgram* FontManager::defaultFontShader;
FT_Library FontManager::ft;
std::unordered_map<std::string, FontFace*> FontManager::Fonts;
Camera* FontManager::screenSpaceCamera;

bool FontManager::init()
{
	defaultFontShader = ResourceManager::addShaderProgramVertFrag(SHADER_FONT_DIRECTORY + "font.vert", SHADER_FONT_DIRECTORY + "font.frag");

	if (FT_Init_FreeType(&ft))
	{
		SAT_ERROR_LOC("\n:FREETYPE: Could not init FreeType Library");
		return false;
	}
	screenSpaceCamera = new Camera();
	screenSpaceCamera->orthographic(0.0f, 1920.0f, 0.0f, 1080.0f, -10.0f, 10.0f);
	screenSpaceCamera->update(1.0f);
	loaded = true;
	return true;
}

FontFace* FontManager::initNewFont(const std::string & filepath, int height)
{
	return initNewFont(filepath, 0, height);
}

FontFace* FontManager::initNewFont(const std::string & filepath, int width, int height)
{
	FontFace* newFace;
	std::string hash = filepath + "_" + std::to_string(width) + "_" + std::to_string(height);
	if (Fonts.find(hash) == Fonts.end())
	{
		PRINT_LOG_WARNING("NEW FONT");
		newFace = new FontFace();
		newFace->filename = filepath;

		if (FT_New_Face(ft, ("../assets/fonts/" + filepath).c_str(), 0, &newFace->face))
		{
			SAT_ERROR_LOC("\n:FREETYPE: Failed to load font");
			PRINT_LOG_WARNING("%s", ("../assets/fonts/" + filepath).c_str());
			return false;
		}

		FT_Set_Pixel_Sizes(newFace->face, width, height);

		newFace->init();
		if (newFace->isLoaded())
		{
			Fonts[hash] = newFace;
		}
		else
		{
			PRINT_LOG_ERROR("ERROR INITIALIZING FONT");
		}
	}
	else
	{
		newFace = Fonts.at(hash);
	}

	return newFace;
}

//Commodore Angled.ttf

FontFace::FontFace()
{
}

bool FontFace::init()
{
	PRINT_LOG_SAFE("%s", filename.c_str());
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			SAT_ERROR_LOC("\n:FREETYPE: Failed to load Glyph");
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			vec2((float)face->glyph->bitmap.width, (float)face->glyph->bitmap.rows),
			vec2((float)face->glyph->bitmap_left,  (float)face->glyph->bitmap_top),
			vec2((float)face->glyph->advance.x, (float)face->glyph->advance.y)
		};

		characters.insert(std::pair<GLchar, Character>(c, character));
	}

	loaded = true;
	return true;
}

bool FontFace::isLoaded()
{
	return loaded;
}

void TextRenderer::init()
{
	// Setup VBO

	GLfloat xpos = 0.0f;
	GLfloat ypos = 0.0f;
	GLfloat w = 0.5f;
	GLfloat h = 0.5f;

	GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
	};

	// Set up position (vertex) attribute
	{
		VertexBufferData positionAttrib;
		positionAttrib.attributeLocation = AttributeLocations::VERTEX;
		positionAttrib.attributeName = "vertex";
		positionAttrib.data = &vertices[0];
		positionAttrib.sizeOfElement = sizeof(float);
		positionAttrib.elementType = GL_FLOAT;
		positionAttrib.numElementsPerAttribute = 4;
		positionAttrib.numElements = 6 * 4;
		vao.addVBO(positionAttrib);
	}


	vao.createVAO(GL_DYNAMIC_DRAW);

	shader = FontManager::defaultFontShader;
	loaded = true;
}

void TextRenderer::update(float deltaTime)
{
	time += deltaTime;
	
	float x = 0.0f;
	float y = 0.0f;
	size_t i = 0;
	lineData.clear();
	LineData line;
	line.origin = vec2(0.0f, 0.0f);
	// Don't include newlines as actual characters
	std::string::const_iterator s;
	for (s = text.begin(); s != text.end(); ++s)
	{
		Character ch = fontface->characters[*s];
		if (*s == '\n')
		{
			//x += (ch.offset.x + ch.size.x) * scale;
			line.width = x;
			lineData.push_back(line);
			y -= (ch.advance.y / 64.0f) * scale * lineSpacing;
			x = 0.0f;
			line.origin = vec2(x, y);
			continue;
		}
				
		x += (ch.advance.x / 64.0f) * scale;
		++i;
	}
	line.width = x;
	lineData.push_back(line);	
	data.resize(i);

	switch (alignment)
	{
	case Left:
		x = 0.0f;
		break;
	case Center:
		x = -lineData[0].width * 0.5f;
		break;
	case Right:
		x = -lineData[0].width;
		break;
	}
	y = 0.0f;
	i = 0;
	size_t lineNumber = 0;

	for (s = text.begin(); s != text.end(); ++s)
	{
		Character ch = fontface->characters[*s];
		if (*s == '\n')
		{
			lineNumber++;
			y -= fontface->face->size->metrics.height / 64.0f;
			//y -= (ch.advance.y / 64.0f) * scale * lineSpacing;
			switch (alignment)
			{
			case Left:
				x = 0.0f;
				break;
			case Center:
				x = -lineData[lineNumber].width * 0.5f;
				break;
			case Right:
				x = -lineData[lineNumber].width;
				break;
			}
			continue;
		}

		LetterData newLetter;
		newLetter.x = x + ch.offset.x * scale;
		newLetter.y = y - (ch.size.y - ch.offset.y) * scale;
		newLetter.size = ch.size * scale;

		//calculate shake offset
		newLetter.shakeOffset.x = shake.amount.x * sin(time * shake.speed.x + (x * shake.frequency.x + y * shake.frequency.y));
		newLetter.shakeOffset.y = shake.amount.y * cos(time * shake.speed.y + (x * shake.frequency.x + y * shake.frequency.y));
		newLetter.pos += newLetter.shakeOffset;
		newLetter.pos += origin;

		newLetter.textureID = ch.textureID;

		x += (ch.advance.x / 64.0f) * scale;
		data[i] = newLetter;
		++i;
	}
	
}

void TextRenderer::draw()
{
	if (!loaded)
	{
		SAT_DEBUG_LOG_WARNING("Text Renderer not initialized!");
		return;
	}

	if (localSpace == ScreenSpace)
	{
		FontManager::screenSpaceCamera->bind();
	}

	shader->bind();
	glActiveTexture(GL_TEXTURE0);
	vao.bind();

	//// Iterate through and draw each character
	//// TODO: Optimize and instance
	//std::string::const_iterator s;
	//for(s = text.begin(); s != text.end(); ++s)
	//{ 
	//	Character ch = fontface->characters[*s];
	//
	//	//calculate shake offset
	//	vec2 shakeOffset;
	//	shakeOffset.x = shake.amount.x * sin(time * shake.speed.x + x / shake.frequency.x);
	//	shakeOffset.y = shake.amount.y * cos(time * shake.speed.y + x / shake.frequency.y);
	//
	//	rect box;
	//	box.x = x + ch.offset.x * scale;
	//	box.y = y - (ch.size.y - ch.offset.y) * scale;
	//	box.pos += shakeOffset;
	//
	//	box.w = ch.size.x * scale;
	//	box.h = ch.size.y * scale;
	//	// Update VBO for each character
	//	GLfloat vertices[6][4] = 
	//	{
	//		{ box.x,		 box.y + box.h, 0.0, 0.0 },
	//		{ box.x,		 box.y,			0.0, 1.0 },
	//		{ box.x + box.w, box.y,			1.0, 1.0 },
	//
	//		{ box.x,		 box.y + box.h, 0.0, 0.0 },
	//		{ box.x + box.w, box.y,			1.0, 1.0 },
	//		{ box.x + box.w, box.y + box.h, 1.0, 0.0 }
	//	};
	//	// Render glyph texture over quad
	//	glBindTexture(GL_TEXTURE_2D, ch.textureID);
	//	// Update content of VBO memory
	//	glBindBuffer(GL_ARRAY_BUFFER, vao.getVboHandle(AttributeLocations::VERTEX));
	//	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
	//
	//	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	//	// Render quad
	//	glDrawArrays(GL_TRIANGLES, 0, 6);
	//	// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
	//	x += (ch.advance.x / 64.0f) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	//}
	
	for (LetterData i : data)
	{
		shader->sendUniform("uTextColor", color * i.color);

		// Update VBO for each character
		GLfloat vertices[6][4] =
		{
			{ i.x,		 i.y + i.h, 0.0, 0.0 },
			{ i.x,		 i.y,		0.0, 1.0 },
			{ i.x + i.w, i.y,		1.0, 1.0 },

			{ i.x,		 i.y + i.h, 0.0, 0.0 },
			{ i.x + i.w, i.y,		1.0, 1.0 },
			{ i.x + i.w, i.y + i.h, 1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, i.textureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, vao.getVboHandle(AttributeLocations::VERTEX));
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	vao.unbind();
	shader->unbind();
}
