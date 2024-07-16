#include "Player.h"
#include "Engine.h"

void Player::Update(float dt)
{
	float thrust = 0;
	if (g_engine.GetInput().getKeyDown(SDL_SCANCODE_LEFT))
	{
		m_degreesPerSecond -= m_rotationSpeed;
		if (randomf() < 0.025)
		{
			m_particles.push_back(Particle{ // Add particles for starboard thruster
					(m_transform.translation - (Vector2{5.0f + randomf(-0.8, 0.8), 4.4f} *m_transform.scale).Rotate(m_transform.rotation)),
					m_velocity * 2 + (Vector2{randomf(-1.6, 1.6), -6.0f} *m_transform.scale).Rotate(m_transform.rotation),
					randomf(1, 6),
					Color{1, 1, 1, 0},
					randomf(0.2 * m_transform.scale, 0.6 * m_transform.scale)
				});
		}
	}
	if (g_engine.GetInput().getKeyDown(SDL_SCANCODE_RIGHT))
	{
		if (randomf() < 0.025)
		{
			m_particles.push_back(Particle{ // Add particles for port thruster
					(m_transform.translation - (Vector2{5.0f + randomf(-0.8, 0.8), -4.4f} *m_transform.scale).Rotate(m_transform.rotation)),
					m_velocity * 2 + (Vector2{randomf(-1.6, 1.6), 6.0f} *m_transform.scale).Rotate(m_transform.rotation),
					randomf(1, 6),
					Color{1, 1, 1, 0},
					randomf(0.2 * m_transform.scale, 0.6 * m_transform.scale)
				});
		}
		m_degreesPerSecond += m_rotationSpeed;
	}

	if (g_engine.GetInput().getKeyDown(SDL_SCANCODE_UP))
	{
		thrust = m_speed;
		if (randomf() < 0.1) 
		{
			m_particles.push_back(Particle{ // Add particles for main thruster
					(m_transform.translation - (Vector2{8.4f, 0 + randomf(-1, 1)} *m_transform.scale).Rotate(m_transform.rotation)),
					m_velocity * 2 + (Vector2{-m_speed * 200, randomf(-1.6, 1.6)} *m_transform.scale).Rotate(m_transform.rotation),
					randomf(1, 8),
					Color{randomf(), randomf(0.8f, 1), 1, 0},
					randomf(0.6 * m_transform.scale, 1.6 * m_transform.scale)
			});
		}
	}

	m_transform.rotation += Math::DegToRad(m_degreesPerSecond) * dt;
	m_velocity += Vector2{ thrust, 0.0f }.Rotate(m_transform.rotation);
	m_transform.translation += m_velocity * dt;
	m_transform.translation.x = Math::Wrap(m_transform.translation.x, (float)g_engine.GetRenderer().GetWidth());
	m_transform.translation.y = Math::Wrap(m_transform.translation.y, (float)g_engine.GetRenderer().GetHeight());

	Actor::Update(dt);
}
