#include "Actor.h"
#include "Model.h"
#include "Engine.h"

void Actor::Update(float dt)
{
	m_collisionCooldown -= dt;
	// Check for lifespan
	if (m_lifespan != 0)
	{
		m_lifespan -= dt;
		if (m_lifespan <= 0)
		{
			m_destroyed = true;
		}
	}

	m_transform.translation += (m_velocity * dt);
	m_velocity *= 1.0f / (1.0f + m_damping * dt); // If there were air in space

	for (Particle& particle : m_particles)
	{
		particle.Update(dt);
		
		if (particle.position.x > g_engine.GetRenderer().GetWidth()) particle.position.x = 0.0f;
		if (particle.position.x < 0) particle.position.x = (float)g_engine.GetRenderer().GetWidth();
		if (particle.position.y > g_engine.GetRenderer().GetHeight()) particle.position.y = 0.0f;
		if (particle.position.y < 0) particle.position.y = (float)g_engine.GetRenderer().GetHeight();
		
	}
}

void Actor::Draw(Renderer& renderer)
{
	if (m_destroyed) return;

	if (m_model) // model pointer is not nullptr
	{
		m_model->Draw(renderer, m_transform);
	}
	if (!m_particles.empty()) {
		for (Particle particle : m_particles) {

			particle.Draw(renderer);
		}
	}
}
