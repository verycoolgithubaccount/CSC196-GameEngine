#include "Particle.h"
#include "Engine.h"

void Particle::Initialize(const Data& data)
{
	isActive = true;
	position = data.position;
	velocity = data.velocity;
	lifespan = data.lifespan;
	size = data.size;
	SetColor(data.color);
}

void Particle::Update(float dt)
{
	position = position + (velocity * dt); // Scale velocity by how much time has passed since last frame so it isn't framerate dependant
	position.x = Math::Wrap(position.x, (float)g_engine.GetRenderer().GetWidth());
	position.y = Math::Wrap(position.y, (float)g_engine.GetRenderer().GetHeight());
	if (lifespan > 0) lifespan -= dt;
	isActive = (lifespan > 0);
}

void Particle::Draw(Renderer& renderer) 
{
	if (lifespan > 0 || lifespan == -42)
	{
		renderer.SetColor(color[0], color[1], color[2], color[3]);

		if (lifespan > 3) {
			renderer.DrawRect(position.x, position.y, size, size);
		}
		else {
			renderer.DrawRect(position.x, position.y, size * (lifespan / 3), size * (lifespan / 3));
		}
	}
}
