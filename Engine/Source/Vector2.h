#pragma once
#include <cmath>

using namespace std;

struct Vector2
{
	float x;
	float y;

	Vector2() = default;
	Vector2(float x, float y) { this->x = x; this->y = y; }
	Vector2(int x, int y) { this->x = (float)x; this->y = (float)y; }

	Vector2 operator + (Vector2 v) const { return Vector2{ x + v.x, y + v.y }; };
	Vector2 operator - (Vector2 v) const { return Vector2{ x - v.x, y - v.y }; };
	Vector2 operator * (Vector2 v) const { return Vector2{ x * v.x, y * v.y }; };
	Vector2 operator / (Vector2 v) const { return Vector2{ x / v.x, y / v.y }; };
	// output,  symbol,   input,                      operation              

	Vector2 operator + (float s) const { return Vector2{ x + s, y + s }; };
	Vector2 operator - (float s) const { return Vector2{ x - s, y - s }; };
	Vector2 operator * (float s) const { return Vector2{ x * s, y * s }; };
	Vector2 operator / (float s) const { return Vector2{ x / s, y / s }; };

	float LengthSqr() { return (x * x) + (y * y); }
	float Length() { return sqrt((x * x) + (y * y)); }
};