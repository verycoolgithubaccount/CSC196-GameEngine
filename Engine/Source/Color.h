#pragma once
#include "MathUtils.h"
#include <cmath>

struct Color
{
	float r, g, b, a;

	Color() = default;
	Color(float r, float g, float b, float a) { this->r = r; this->g = g; this->b = b; this->a = a; }
	//Color(int r, int g, int b, int a) { this->r = (float)r; this->g = (float)g; this->b = (float)b; this->a = (float)a; }

	float operator [] (unsigned int index) const { return (&r)[index]; }
	float& operator [] (unsigned int index) { return (&r)[index]; }

	Color operator + (const Color& c) const { return Color{ r + c.r, g + c.g, b + c.b, a + c.a }; };
	Color operator - (const Color& c) const { return Color{ r - c.r, g - c.g, b - c.b, a - c.a }; };
	Color operator * (const Color& c) const { return Color{ r * c.r, g * c.g, b * c.b, a * c.a }; };
	Color operator / (const Color& c) const { return Color{ r / c.r, g / c.g, b / c.b, a / c.a }; };
	// output,  symbol,   input,                      operation              

	Color operator + (float s) const { return Color{ r + s, g + s, b + s, a + s }; };
	Color operator - (float s) const { return Color{ r - s, g - s, b - s, a - s }; };
	Color operator * (float s) const { return Color{ r * s, g * s, b * s, a * s }; };
	Color operator / (float s) const { return Color{ r / s, g / s, b / s, a / s }; };
};