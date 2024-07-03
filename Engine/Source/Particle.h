#pragma once
#include "Vector2.h"
#include "Renderer.h"

struct Particle
{
	Vector2 position{ 0, 0 };
	Vector2 velocity{ 0, 0 };
	float lifespan = 0;

	Particle() = default;
	Particle(Vector2 position, Vector2 velocity) :
		position{ position },
		velocity{ velocity }
	{}

	void Update(float dt); // delta time - time elapsed since last frame
	void Draw(Renderer& renderer);
};