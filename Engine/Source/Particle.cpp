#include "Particle.h"
void Particle::Update(float dt)
{
	position = position + (velocity * dt); // Scale velocity by how much time has passed since last frame so it isn't framerate dependant
	if (lifespan > 0) lifespan -= dt;
}

void Particle::Draw(Renderer& renderer) 
{
	if (lifespan > 0)
	{
		renderer.SetColor(color1[0], color1[1], color1[2], color1[3]);

		if (lifespan > 3) {
			renderer.DrawRect(position.x, position.y, size, size);
		}
		else {
			renderer.DrawRect(position.x, position.y, size * (lifespan / 3), size * (lifespan / 3));
		}
	}
}
