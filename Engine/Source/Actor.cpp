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
}

void Actor::Draw(Renderer& renderer)
{
	if (m_destroyed) return;

	if (m_model) // model pointer is not nullptr
	{
		m_model->Draw(renderer, m_transform);
	}
}
