#include "FontMatrixScrolling.h"
#include "AniMath.h"


MatrixTextRow::MatrixTextRow(std::vector<char>* choices)
{
	text.resize(50);
	size_t numRand = choices->size();
	for (int i = 0; i < 30; ++i)
	{
		text[i] = choices->at(rand() % numRand);
	}
	shinePosition = rand() % 50;
}

MatrixTextWave::MatrixTextWave()
{
	offset = random(0.0f, 6.28f);
	speed = random(0.5f, 2.0f);
	amount = random(0.2f, 1.0f);;
}

void MatrixTextWave::update(float dt)
{
	offset += speed * dt;
}

float MatrixTextWave::calculateRow(float rowOffset)
{
	return amount * sinf(offset + rowOffset);
}

FontMatrixScrolling::FontMatrixScrolling()
{
	characters.push_back('0');
	characters.push_back('1');
}

void FontMatrixScrolling::init()
{
	for (auto i = 0; i < 20; ++i)
	{
		rows.push_back(MatrixTextRow(&characters));
	}
	for (auto i = 0; i < 20; ++i)
	{
		waves.push_back(MatrixTextWave());
	}
	
	TextRenderer::init();
}

void FontMatrixScrolling::update(float deltatime)
{
	time += deltatime;

	for (size_t i = 0; i < waves.size(); ++i)
	{
		waves[i].update(deltatime);
	}

	for (size_t k = 0; k < rows.size(); ++k)
	{
		float totalOffset = 0.0f;
		for (size_t w = 0; w < waves.size(); ++w)
		{
			totalOffset += waves[k].calculateRow(rowWaveBias * w);
		}







		float x = 0.0f;
		float y = 0.0f;
		lineData.clear();
		size_t s = 0;
		LineData line;
		line.origin = vec2(0.0f, 0.0f);
		// Don't include newlines as actual characters
		for (s = 0; s < rows[k].text.length(); ++s)
		{
			Character ch = fontface->characters[rows[k].text[s]];
			if (rows[k].text[s] == '\n')
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
		}
		line.width = x;
		lineData.push_back(line);
		data.resize(s);

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
		size_t lineNumber = 0;

		for (s = 0; s < rows[k].text.length(); ++s)
		{
			Character ch = fontface->characters[rows[k].text[s]];
			if (rows[k].text[s] == '\n')
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
			data[k] = newLetter;
		}






	}
}