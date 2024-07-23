#pragma once
#include "MathUtils.h"
#include <cmath>
#include <stdint.h>

struct Color
{
	float r, g, b, a;

	Color() = default;
	Color(float r, float g, float b, float a = 1) // setting a default since alpha is commonly 1
	{ this->r = r; this->g = g; this->b = b; this->a = a; }
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

	Color& operator += (const Color& c) { r += c.r; g += c.g; b += c.b; a += c.a; return *this; }
	Color& operator -= (const Color& c) { r -= c.r; g -= c.g; b -= c.b; a -= c.a; return *this; }
	Color& operator *= (const Color& c) { r *= c.r; g *= c.g; b *= c.b; a *= c.a; return *this; }
	Color& operator /= (const Color& c) { r /= c.r; g /= c.g; b /= c.b; a /= c.a; return *this; }
	
	Color& operator += (float s) { r += s; g += s; b += s; a += s; return *this; }
	Color& operator -= (float s) { r -= s; g -= s; b -= s; a -= s; return *this; }
	Color& operator *= (float s) { r *= s; g *= s; b *= s; a *= s; return *this; }
	Color& operator /= (float s) { r /= s; g /= s; b /= s; a /= s; return *this; }

	static uint8_t ToInt(float value) { return static_cast<uint8_t>(Math::Clamp(value, 0.0f, 1.0f) * 255); } // static_cast casts at compiletime
};