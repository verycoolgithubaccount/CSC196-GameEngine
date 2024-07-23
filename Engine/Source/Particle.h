#pragma once
#include "Vector2.h"
#include "Renderer.h"
#include "Color.h"

struct Particle
{
	Vector2 position{ 0, 0 };
	Vector2 velocity{ 0, 0 };
	float lifespan = 0;
	float fadetime = 0;
	uint8_t color1[4] = {255, 255, 255, 0};
	float size = 4;

	Particle() = default;
	Particle(Vector2 position, Vector2 velocity) :
		position{ position },
		velocity{ velocity }
	{}
	Particle(Vector2 position, Vector2 velocity, float lifespan) :
		position{ position },
		velocity{ velocity },
		lifespan{ lifespan }
	{}
	Particle(Vector2 position, Vector2 velocity, float lifespan, Color color, float size) :
		position{ position },
		velocity{ velocity },
		lifespan{ lifespan },
		color1{ Color::ToInt(color.r), Color::ToInt(color.g), Color::ToInt(color.b), Color::ToInt(color.a) },
		size{ size }
	{}

	void Update(float dt); // delta time - time elapsed since last frame
	void Draw(Renderer& renderer);
	void SetColor(Color color) { color1[0] = Color::ToInt(color.r), color1[1] = Color::ToInt(color.g), color1[2] = Color::ToInt(color.b), color1[3] = Color::ToInt(color.a); }
};