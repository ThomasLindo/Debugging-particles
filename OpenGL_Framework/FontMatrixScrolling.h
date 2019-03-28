#pragma once
#include "Font.h"

struct MatrixTextRow
{
	MatrixTextRow(std::vector<char>* choices);
	std::string text;
	int shinePosition;
};

struct MatrixTextWave
{
	MatrixTextWave();
	void update(float dt);
	float calculateRow(float rowOffset);
	float offset;
	float speed;
	float amount;
	float value;
};

class FontMatrixScrolling : public TextRenderer
{
public:
	FontMatrixScrolling();
	std::vector<MatrixTextRow> rows;
	std::vector<MatrixTextWave> waves;
	std::vector<char> characters;
	void init();
	void update(float deltatime);
	float scrollSpeed = 2.0f;
	int averageLength = 15;
	float rowWaveBias = 0.5f;

};