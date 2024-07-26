#pragma once
#include "Vector2.h"
#include "Renderer.h"
#include "Color.h"

struct Particle
{
	struct Data
	{
		Vector2 position{ 0, 0 };
		Vector2 velocity{ 0, 0 };
		float lifespan = 0;
		Color color{1, 1, 1};
		float size = 1;
	};

	Vector2 position{ 0, 0 };
	Vector2 velocity{ 0, 0 };
	float lifespan = 0;
	uint8_t color[4] = {255, 255, 255, 0};
	float size = 4;
	bool isActive{ false };

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
		color{ Color::ToInt(color.r), Color::ToInt(color.g), Color::ToInt(color.b), Color::ToInt(color.a) },
		size{ size }
	{}

	void Initialize(const Data& data);

	void Update(float dt); // delta time - time elapsed since last frame
	void Draw(Renderer& renderer);
	void SetColor(Color new_color) { color[0] = Color::ToInt(new_color.r), color[1] = Color::ToInt(new_color.g), color[2] = Color::ToInt(new_color.b), color[3] = Color::ToInt(new_color.a); }
};