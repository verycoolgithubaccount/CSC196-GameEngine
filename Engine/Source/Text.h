#pragma once
#include "Color.h"
#include <SDL_ttf.h>
#include<iostream>

class Font;
class Renderer;

class Text
{
private:
	Font* m_font{ nullptr };
	SDL_Texture* m_texture{ nullptr };
public:
	Text() = default;
	Text(Font* font) : m_font{ font } {}
	~Text();

	bool Create(Renderer& renderer, const std::string& text, const Color& color);
	void Draw(Renderer& renderer, int x, int y);
};