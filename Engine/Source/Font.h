#pragma once
#include <SDL_ttf.h>
#include<iostream>

class Font
{
private:
	_TTF_Font* m_ttfFont{ nullptr };
public:
	Font() = default;
	~Font();

	bool Load(const std::string& name, int fontSize);
	
	friend class Text;
};