#include "Player.h"
#include "Engine.h"
#include "Bolt.h"
#include "Scene.h"
#include "GameData.h"
#include "SpaceGame.h"
#include <iostream>

void Player::Update(float dt)
{
	float thrust = 0;
	if (g_engine.GetInput().getKeyDown(SDL_SCANCODE_LEFT) || g_engine.GetInput().getKeyDown(SDL_SCANCODE_A))
	{
		m_degreesPerSecond -= m_rotationSpeed;
		if (randomf() < 0.1)
		{
			m_particles.push_back(Particle{ // Add particles for starboard thruster
					(m_transform.translation - (Vector2{5.0f + randomf(-0.8f, 0.8f), 4.4f} *m_transform.scale).Rotate(m_transform.rotation)),
					m_velocity + (Vector2{randomf(-1.6f, 1.6f), -6.0f} *m_transform.scale).Rotate(m_transform.rotation),
					randomf(1, 6),
					Color{1, 1, 1},
					randomf(0.2f * m_transform.scale, 0.6f * m_transform.scale)
				});
		}
	}
	if (g_engine.GetInput().getKeyDown(SDL_SCANCODE_RIGHT) || g_engine.GetInput().getKeyDown(SDL_SCANCODE_D))
	{
		if (randomf() < 0.1)
		{
			m_particles.push_back(Particle{ // Add particles for port thruster
					(m_transform.translation - (Vector2{5.0f + randomf(-0.8f, 0.8f), -4.4f} *m_transform.scale).Rotate(m_transform.rotation)),
					m_velocity + (Vector2{randomf(-1.6f, 1.6f), 6.0f} *m_transform.scale).Rotate(m_transform.rotation),
					randomf(1, 6),
					Color{1, 1, 1},
					randomf(0.2f * m_transform.scale, 0.6f * m_transform.scale)
				});
		}
		m_degreesPerSecond += m_rotationSpeed;
	}

	if (g_engine.GetInput().getKeyDown(SDL_SCANCODE_UP) || g_engine.GetInput().getKeyDown(SDL_SCANCODE_W))
	{
		thrust = m_speed;

		if (randomf() < 0.5) 
		{
			m_particles.push_back(Particle{ // Add particles for main thruster
					(m_transform.translation - (Vector2{8.4f, 0 + randomf(-1, 1)} *m_transform.scale).Rotate(m_transform.rotation)),
					m_velocity + (Vector2{-m_speed * 100, randomf(-3, 3)} *m_transform.scale).Rotate(m_transform.rotation),
					randomf(1, 3),
					Color{randomf(), randomf(0.8f, 1), 1},
					randomf(0.8f * m_transform.scale, 1.8f * m_transform.scale)
			});
		}
	}
	else if (m_ThrusterSoundTimer > 0) {
		m_ThrusterSoundTimer = 0;
	}

	m_transform.rotation += Math::DegToRad(m_degreesPerSecond) * dt;
	m_velocity += Vector2{ thrust, 0.0f }.Rotate(m_transform.rotation);

	// fire
	m_fireTimer -= dt;
	if ((g_engine.GetInput().getKeyDown(SDL_SCANCODE_SPACE) || g_engine.GetInput().getKeyDown(SDL_SCANCODE_E)) && m_fireTimer <= 0)
	{
		m_fireTimer = 0.3f * m_fireModifier;

		// actor
		Model* model = new Model{ GameData::boltPoints, Color{ 0.0f, 1.0f, 1.0f } };
		Transform transform{ m_transform.translation + (Vector2{ 5.0f, 3.6f } * m_transform.scale).Rotate(m_transform.rotation), m_transform.rotation, m_transform.scale / 10 };

		Bolt* bolt = new Bolt(500, m_velocity, transform, model);
		bolt->SetLifeSpan(1);
		bolt->SetTag("Allied Bullet");
		m_scene->AddActor(bolt);
	}

	Actor::Update(dt);
	m_transform.translation.x = Math::Wrap(m_transform.translation.x, (float)g_engine.GetRenderer().GetWidth());
	m_transform.translation.y = Math::Wrap(m_transform.translation.y, (float)g_engine.GetRenderer().GetHeight());
}

void Player::OnCollision(Actor* collider)
{
	if (collider->GetTag() == "Enemy")
	{
		m_destroyed = true;
		dynamic_cast<SpaceGame*>(m_scene->GetGame())->OnPlayerDeath();
	}
}
