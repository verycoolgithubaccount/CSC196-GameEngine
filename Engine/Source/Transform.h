#pragma once
#include "Vector2.h"

struct Transform
{
	Vector2 translation;
	float rotation;
	float scale;

	Transform() = default;
	Transform(Vector2 position = { 0, 0 }, float rotation = 0, float scale = 1) :
		translation{ position },
		rotation{ rotation },
		scale{ scale }
	{}
};