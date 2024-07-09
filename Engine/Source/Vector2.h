#pragma once
#include "MathUtils.h"
#include <cmath>

struct Vector2
{
	float x;
	float y;

	Vector2() = default;
	Vector2(float x, float y) { this->x = x; this->y = y; }
	Vector2(int x, int y) { this->x = (float)x; this->y = (float)y; }

	float operator [] (unsigned int index) const { return (&x)[index]; } // 0 is x, 1 will go forward to the next variable in memory, y
	float& operator [] (unsigned int index) { return (&x)[index]; } // same as above but lets you edit the values

	Vector2 operator + (const Vector2& v) const { return Vector2{ x + v.x, y + v.y }; };
	Vector2 operator - (const Vector2& v) const { return Vector2{ x - v.x, y - v.y }; };
	Vector2 operator * (const Vector2& v) const { return Vector2{ x * v.x, y * v.y }; };
	Vector2 operator / (const Vector2& v) const { return Vector2{ x / v.x, y / v.y }; };
	// output,  symbol,   input,                      operation              

	Vector2 operator + (float s) const { return Vector2{ x + s, y + s }; };
	Vector2 operator - (float s) const { return Vector2{ x - s, y - s }; };
	Vector2 operator * (float s) const { return Vector2{ x * s, y * s }; };
	Vector2 operator / (float s) const { return Vector2{ x / s, y / s }; };

	float LengthSqr() const { return (x * x) + (y * y); }
	float Length() const { return Math::Sqrt((x * x) + (y * y)); }

	float Angle() const { return Math::Atan2(y, x); }

	Vector2 Rotate(float radians) const;
};

inline Vector2 Vector2::Rotate(float radians) const
{
	float x_ = x * cos(radians) - y * sin(radians);
	float y_ = x * sin(radians) + y * cos(radians);

	return { x_, y_ };
}