#include "Particle.h"

void Particle::Update(float dt)
{
	position = position + (velocity * dt); // Scale velocity by how much time has passed since last frame so it isn't framerate dependant
}

void Particle::Draw(Renderer& renderer)
{
	renderer.DrawPoint(position);
}
