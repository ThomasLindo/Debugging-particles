#pragma once
#pragma comment(lib, "freetype.lib")

// Can't have it in a new directory because of #defines setting up include directories
// Could maybe solve this later by setting up a separate include directory if it's an issue
#include <ft2build.h>
#include FT_FREETYPE_H

#include <GL/glew.h>
#include "ShaderProgram.h"
#include "VertexBufferObject.h"
#include <gmtk/gmtk.h>
#include <unordered_map>
#include "Transform.h"
#include <SatMath/Rect.h>

#define SHADER_FONT_DIRECTORY std::string("font/")
#define ASSETS_FONT_DIRECTORY std::string("fonts/")

struct Character
{
	GLuint textureID;	// ID Handle
	vec2 size;	// Dimensions
	vec2 offset;	// Offset to left/top of glyph
	vec2 advance; // Horizontal offset to advance to next glyph
};

class FontFace;
class TextRenderer;

class FontManager
{
public:
	static bool init();

	static ShaderProgram* defaultFontShader;

	static std::unordered_map<std::string, FontFace*> Fonts;
	static FontFace* initNewFont(const std::string& filepath, int height);
	static FontFace* initNewFont(const std::string& filepath, int width, int height);
	static Camera* screenSpaceCamera;
private:
	static FT_Library ft;
	static bool loaded;
};

class FontFace
{
public:
	FontFace();
	bool init();
	bool isLoaded();

	std::unordered_map<GLchar, Character> characters;
	std::string filename;
	FT_Face face;
private:
	bool loaded = false;
};

struct LetterData : rect
{
	vec2 shakeOffset; 
	float scale = 1.0f;
	float scaleInd = 1.0f; // This scale doesn't affect spacing
	GLuint textureID;
	vec4 color = vec4(1.0f);
};

struct LineData
{
	vec2 origin = vec2(0.0f);
	float width;
};

struct TextShake
{
	vec2 amount = vec2(0.0f, 0.0f);
	vec2 frequency = vec2(0.0f, 0.0f );
	vec2 speed = vec2(0.0f);
};

class TextRenderer
{
public:
	enum Alignment
	{
		Left,
		Center,
		Right
	};

	enum LocalSpace
	{
		ScreenSpace,
		WorldSpace
	};

	VertexArrayObject vao;
	ShaderProgram* shader;
	FontFace* fontface;
	vec4 color = vec4(1.0f);
	std::string text;
	std::vector<LetterData> data;
	LocalSpace localSpace = ScreenSpace;
	Alignment alignment = Left;
	Transform transform;
	float scale = 1.0f;
	float lineSpacing = 1.0f;
	vec2 origin;
	std::vector<LineData> lineData;

	TextShake shake;
	float time = 0.0f;
	void init();
	void update(float deltaTime);
	void draw();

	bool loaded = false;
};


