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
	if (!m_controlsDisabled && (g_engine.GetInput().getKeyDown(SDL_SCANCODE_LEFT) || g_engine.GetInput().getKeyDown(SDL_SCANCODE_A)))
	{
		m_degreesPerSecond -= m_rotationSpeed;
		if (randomf() < 0.1)
		{
			Particle::Data data
			{
				(m_transform.translation - (Vector2{5.0f + randomf(-0.8f, 0.8f), 4.4f} *m_transform.scale).Rotate(m_transform.rotation)),
				m_velocity + (Vector2{randomf(-1.6f, 1.6f), -6.0f} *m_transform.scale).Rotate(m_transform.rotation),
				randomf(1, 6),
				Color{1, 1, 1},
				randomf(0.2f * m_transform.scale, 0.6f * m_transform.scale)
			};
			g_engine.GetParticleSystem().AddParticle(data);
		}
	}
	if (!m_controlsDisabled && (g_engine.GetInput().getKeyDown(SDL_SCANCODE_RIGHT) || g_engine.GetInput().getKeyDown(SDL_SCANCODE_D)))
	{
		if (randomf() < 0.1)
		{
			Particle::Data data
			{
				(m_transform.translation - (Vector2{5.0f + randomf(-0.8f, 0.8f), -4.4f} *m_transform.scale).Rotate(m_transform.rotation)),
				m_velocity + (Vector2{randomf(-1.6f, 1.6f), 6.0f} *m_transform.scale).Rotate(m_transform.rotation),
				randomf(1, 6),
				Color{1, 1, 1},
				randomf(0.2f * m_transform.scale, 0.6f * m_transform.scale)
			};
			g_engine.GetParticleSystem().AddParticle(data);
		}
		m_degreesPerSecond += m_rotationSpeed;
	}

	if (!m_controlsDisabled && (g_engine.GetInput().getKeyDown(SDL_SCANCODE_UP) || g_engine.GetInput().getKeyDown(SDL_SCANCODE_W)))
	{
		thrust = m_speed;

		if (randomf() < 0.5) 
		{
			Particle::Data data
			{
				(m_transform.translation - (Vector2{8.4f, 0 + randomf(-1, 1)} *m_transform.scale).Rotate(m_transform.rotation)),
				m_velocity + (Vector2{-m_speed * 100, randomf(-3, 3)} *m_transform.scale).Rotate(m_transform.rotation),
				randomf(1, 3),
				Color{randomf(), randomf(0.8f, 1), 1},
				randomf(0.8f * m_transform.scale, 1.8f * m_transform.scale)
			};
			g_engine.GetParticleSystem().AddParticle(data);
		}
	}
	else if (m_ThrusterSoundTimer > 0) {
		m_ThrusterSoundTimer = 0;
	}

	m_transform.rotation += Math::DegToRad(m_degreesPerSecond) * dt;
	m_velocity += Vector2{ thrust, 0.0f }.Rotate(m_transform.rotation);

	// fire
	m_fireTimer -= dt;
	if (!m_controlsDisabled && (g_engine.GetInput().getKeyDown(SDL_SCANCODE_SPACE) || g_engine.GetInput().getKeyDown(SDL_SCANCODE_E)) && m_fireTimer <= 0)
	{
		m_fireTimer = 0.2f * m_fireModifier;

		// actor
		Model* model = new Model{ GameData::boltPoints, Color{ 0.0f, 1.0f, 1.0f } };
		Transform transform{ m_transform.translation + (Vector2{ 5.0f, 3.6f } * m_transform.scale).Rotate(m_transform.rotation), m_transform.rotation, m_transform.scale / 10 };

		auto bolt = std::make_unique<Bolt>(500, m_velocity, transform, model);
		bolt->SetLifeSpan(5);
		bolt->SetTag("Player Bullet");
		m_scene->AddActor(std::move(bolt));
		g_engine.GetAudio().PlaySound("bolt_fire.wav");
	}

	SpaceGame* game = dynamic_cast<SpaceGame*>(m_scene->GetGame());
	if (!m_controlsDisabled && g_engine.GetInput().getKeyDown(SDL_SCANCODE_Q) && game->GetLaserEnergy() == game->GetMaxLaserEnergy() && !m_firingLaser)
	{
		m_firingLaser = true;
		g_engine.GetAudio().PlaySound("laser.wav");
	}
	else if (!m_controlsDisabled && g_engine.GetInput().getKeyDown(SDL_SCANCODE_Q) && game->GetLaserEnergy() > 0.1 && m_firingLaser)
	{
		game->AddLaserEnergy(-game->GetMaxLaserEnergy() * 0.66 * dt);
		ShootLaser(
			m_transform.translation,
			m_transform.translation + (Vector2{ 5.0f, -3.6f } *m_transform.scale).Rotate(m_transform.rotation),
			(Vector2{ 1, 0 }).Rotate(m_transform.rotation).Normalized() * 10,
			{ 0, 1, 1 }, "Player Laser");
	}
	else m_firingLaser = false;


	Actor::Update(dt);
	m_transform.translation.x = Math::Wrap(m_transform.translation.x, (float)g_engine.GetRenderer().GetWidth());
	m_transform.translation.y = Math::Wrap(m_transform.translation.y, (float)g_engine.GetRenderer().GetHeight());
}

void Player::OnCollision(Actor* collider)
{
	SpaceGame* game = dynamic_cast<SpaceGame*>(m_scene->GetGame());
	if (collider->GetTag() == "Enemy Bullet")
	{
		m_velocity += ((collider->GetVelocity() - m_velocity) * .1);
		g_engine.GetAudio().PlaySound("boom.wav");

		game->OnPlayerDamage(1);

		if (game->GetHealth() <= 0 && !m_controlsDisabled) dynamic_cast<SpaceGame*>(m_scene->GetGame())->OnPlayerDeath();
	}
	else if (collider->GetTag() == "Enemy" || collider->GetTag() == "Ally")
	{
		if (m_collisionCooldown < 0) {
			m_collisionCooldown = 0.1;
			Vector2 directionToCollider = (collider->GetTransform().translation - m_transform.translation).Normalized();
			Vector2 relativeVelocity = collider->GetVelocity() - m_velocity;

			float magnitude = (relativeVelocity.x * directionToCollider.x) + (relativeVelocity.y * directionToCollider.y)
				/ Math::Sqrt((directionToCollider.x * directionToCollider.x) + (directionToCollider.y * directionToCollider.y));

			m_velocity += (directionToCollider)*magnitude;
			if (relativeVelocity.Length() > 40) {
				g_engine.GetAudio().PlaySound("ship_collision.wav");
			}

			game->OnPlayerDamage(int(relativeVelocity.Length() / 200));

			if (game->GetHealth() <= 0 && !m_controlsDisabled) dynamic_cast<SpaceGame*>(m_scene->GetGame())->OnPlayerDeath();
		}
	}
}

void Player::OnDeath()
{
	for (int i = 0; i < 400; i++) // Explosion
	{
		Particle::Data data
		{
			m_transform.translation,
			m_velocity + (randomOnUnitCircle() * randomf(0, 300)),
			randomf(0, 1),
			Color{ 1, randomf(), randomf(0.0f, 0.3f) },
			randomf(5.0f * m_transform.scale, 10.0f * m_transform.scale)
		};
		g_engine.GetParticleSystem().AddParticle(data);
	}
	for (int i = 0; i < 100; i++) // Ring
	{
		Particle::Data data2
		{
			m_transform.translation,
			m_velocity + (randomOnUnitCircle() * 450),
			randomf(0, 1),
			Color{ 1, 1, 1 },
			randomf(2.8f * m_transform.scale, 3.8f * m_transform.scale)
		};
		g_engine.GetParticleSystem().AddParticle(data2);
	}
	for (int i = 0; i < 50; i++)  // Pieces
	{
		Particle::Data data
		{
			m_transform.translation,
			m_velocity + (randomOnUnitCircle() * randomf(500, 550)),
			randomf(0, 1),
			Color{0.0f, 0.25f, 0.25f},
			randomf(10.0f * m_transform.scale, 20.0f * m_transform.scale)
		};
		g_engine.GetParticleSystem().AddParticle(data);
	}
	m_destroyed = true;
}

void Player::HitByRay(std::string rayTag)
{
}

void Player::ShootLaser(Vector2 originPosition, Vector2 position, Vector2 direction, Color color, std::string rayTag)
{
	if (randomf() > 0.9) {
		Particle::Data data
		{
			position,
			Vector2{0,0},
			randomf(0.01, 0.05),
			color,
			randomf(300.0f, 500.0f)
		};
		g_engine.GetParticleSystem().AddParticle(data);
	}

	if (!m_scene->CheckHitByRay(originPosition, position, rayTag) && position.x > 0 && position.x < g_engine.GetRenderer().GetWidth()
		&& position.y > 0 && position.y < g_engine.GetRenderer().GetHeight())
	{
		ShootLaser(originPosition, position + direction, direction, color, rayTag);
	}
}
