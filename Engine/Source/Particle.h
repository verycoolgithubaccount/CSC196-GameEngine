#pragma once
#include "Vector2.h"
#include "Renderer.h"

struct Particle
{
	Vector2 position{ 0, 0 };
	Vector2 velocity{ 0, 0 };
	float lifespan = 0;
	uint8_t red, green, blue, alpha = 0;

	Particle() = default;
	Particle(Vector2 position, Vector2 velocity) :
		position{ position },
		velocity{ velocity },
		red{ 255 },
		green{ 255 },
		blue{ 255 },
		alpha{ 255 }
	{}
	Particle(Vector2 position, Vector2 velocity, float lifespan) :
		position{ position },
		velocity{ velocity },
		lifespan{ lifespan },
		red{ 255 },
		green{ 255 },
		blue{ 255 },
		alpha{ 255 }
	{}
	Particle(Vector2 position, Vector2 velocity, float lifespan, uint8_t color[4]) :
		position{ position },
		velocity{ velocity },
		lifespan{ lifespan },
		red{ color[0] },
		green{ color[1] },
		blue{ color[2] },
		alpha{ color[3] }
	{}

	void Update(float dt); // delta time - time elapsed since last frame
	void Draw(Renderer& renderer);
};