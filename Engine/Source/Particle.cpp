#include "Particle.h"

void Particle::Update(float dt)
{
	position = position + (velocity * dt); // Scale velocity by how much time has passed since last frame so it isn't framerate dependant
	if (lifespan != 0) lifespan -= dt;
}

void Particle::Draw(Renderer& renderer) 
{
	if (lifespan > 0)
	{
		renderer.SetColor(red, green, blue, alpha);
		renderer.DrawRect(position.x, position.y, size, size);
	}
}
