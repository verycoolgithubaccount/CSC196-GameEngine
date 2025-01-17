#include "Enemy.h"
#include "Player.h"
#include "Scene.h"
#include "Engine.h"
#include "GameData.h"
#include "Bolt.h"
#include "Game.h"
#include <iostream>

void Enemy::Update(float dt)
{
	if (m_health <= 0 && m_movementState != MovementState::DEAD) {
		m_deathTimer = 3;
		m_movementState = MovementState::DEAD;
	}

	if (m_movementState == MovementState::DEAD)
	{
		if (randomf() > 0.983f) {
			Vector2 direction = Vector2{ randomf(-1, 1), randomf(-1, 1) }.Normalized();
			for (int i = 0; i < 50; i++)
			{
				Particle::Data data
				{
					m_transform.translation + ((direction * 4 * m_transform.scale) + randomf(0, 3)),
					m_velocity + (direction * random(0, 200)) + randomf(0, 10),
					randomf(0, 1),
					Color{ 1, randomf(), randomf(0.0f, 0.3f) },
					randomf(2.8f * m_transform.scale, 3.8f * m_transform.scale)
				};
				g_engine.GetParticleSystem().AddParticle(data);
			}
		}

		m_deathTimer -= dt;
		if (m_deathTimer <= 0) {
			OnDeath();
		}
	} 
	else {
		Vector2 targetPosition = m_scene->GetNearestAlliedPosition(m_transform.translation);
		Vector2 targetVelocity = m_scene->GetNearestAlliedVelocity(m_transform.translation);
		if (!(targetPosition == Vector2{ (float)HUGE_VAL, (float)HUGE_VAL }))
		{
			Vector2 directionToTarget = targetPosition - m_transform.translation;
			//m_velocity = directionToTarget.Normalized() * m_speed * dt;
			//m_transform.rotation = direction.Angle();
			float rotationDifference = directionToTarget.Angle() - m_transform.rotation;

			switch (m_movementState) {
			case MovementState::SENTRY:
				//std::cout << "SENTRY" << std::endl;
				if (!(m_lastTargetVelocity == targetVelocity)) m_movementState = MovementState::FOLLOWING;
				else Rotate(directionToTarget, rotationDifference, dt);
				break;
			case MovementState::TRAVELLING: {
				bool willOvershoot = false;
				float stepsUntilRotationGoal = (Math::Abs((Vector2{ 0, 0 } - directionToTarget).Angle() / (Math::DegToRad(m_degreesPerSecond)))) * dt;
				float stepsUntilPositionGoal = ((targetPosition.Distance(m_transform.translation)) / (Math::Abs((targetVelocity - m_velocity).Length())));

				if (m_speed * stepsUntilPositionGoal <= Math::Abs((targetVelocity - m_velocity).Length())) willOvershoot = true;

				if (targetPosition.Distance(m_transform.translation) < 100) {
					/*Vector2 oppositeDirectionFromTarget = Vector2{0, 0} - (player->GetTransform().translation - m_transform.translation);
					float oppositeRotationDifference = oppositeDirectionFromTarget.Angle() - m_transform.rotation;

					if (Math::Abs(oppositeDirectionFromTarget.Angle() - m_transform.rotation) > 0.1f)
					{
						Rotate(oppositeDirectionFromTarget, oppositeRotationDifference, dt);
					}
					if (Math::Abs(oppositeDirectionFromTarget.Angle() - m_transform.rotation) < Math::DegToRad(10))
					{
						FireMainThruster(dt);
					}
					*/
					m_movementState = MovementState::FOLLOWING;
				}
				else {
					if (Math::Abs(directionToTarget.Angle() - m_transform.rotation) > 0.1f)
					{

						Rotate(directionToTarget, rotationDifference, dt);
					}
					if (Math::Abs(directionToTarget.Angle() - m_transform.rotation) < Math::DegToRad(10)) FireMainThruster(dt);
				}


				//std::cout << "TRAVELLING" << std::endl;
				break;
			}
			case MovementState::FOLLOWING: {
				//std::cout << "FOLLOWING" << std::endl;
				Vector2 playerVelocity = targetVelocity;
				Vector2 velocityDiff = targetVelocity - m_velocity;

				if (Math::Abs(velocityDiff.x) < m_speed + 0.5 && Math::Abs(velocityDiff.y) < m_speed + 0.5)
				{
					m_lastTargetVelocity = playerVelocity;
					m_movementState = MovementState::SENTRY;
				}
				else if (Math::Abs(velocityDiff.Angle() - m_transform.rotation) > 0.1f)
				{
					Rotate(velocityDiff, velocityDiff.Angle() - m_transform.rotation, dt);
				}
				else {
					FireMainThruster(dt);
				}
				break;
			}
			default:
				break;
			}

			m_fireTimer -= dt;
			if (Math::Abs(rotationDifference) < 0.5 && m_fireTimer <= 0)
			{
				m_fireTimer = 0.5;
				// actor
				Model* model = new Model{ GameData::boltPoints, Color{ 1.0f, 0.0f, 0.0f } };
				Transform transform{ m_transform.translation + (Vector2{ 5.0f, 3.6f } *m_transform.scale).Rotate(m_transform.rotation), m_transform.rotation, m_transform.scale / 10 };

				auto bolt = std::make_unique<Bolt>(500, m_velocity, transform, model);
				bolt->SetLifeSpan(5);
				bolt->SetTag("Enemy Bullet");
				m_scene->AddActor(std::move(bolt));
			}
		}
	}

	m_transform.rotation += Math::DegToRad(m_degreesPerSecond) * dt;

	Actor::Update(dt);
	m_transform.translation.x = Math::Wrap(m_transform.translation.x, (float)g_engine.GetRenderer().GetWidth());
	m_transform.translation.y = Math::Wrap(m_transform.translation.y, (float)g_engine.GetRenderer().GetHeight());
}

void Enemy::FireStarboardThruster()
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

void Enemy::FirePortThruster()
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

void Enemy::FireMainThruster(float dt)
{
	if (randomf() < 0.5)
	{
		Particle::Data data
		{
			(m_transform.translation - (Vector2{8.4f, 0 + randomf(-1, 1)} *m_transform.scale).Rotate(m_transform.rotation)),
			m_velocity + (Vector2{-m_speed * 100, randomf(-3, 3)} *m_transform.scale).Rotate(m_transform.rotation),
			randomf(1, 3),
			Color{1, randomf(), randomf(0.0f, 0.3f)},
			randomf(0.8f * m_transform.scale, 1.8f * m_transform.scale)
		};
		g_engine.GetParticleSystem().AddParticle(data);
	}
	m_velocity += Vector2{ m_speed, 0.0f }.Rotate(m_transform.rotation);
}

void Enemy::Rotate(Vector2 directionToTarget, float rotationGoal, float dt)
{
	float stepsUntilRotationGoal = 0;
	bool willOvershoot = false;
	if (m_degreesPerSecond != 0)
	{
		stepsUntilRotationGoal = Math::Abs((rotationGoal / (Math::DegToRad(m_degreesPerSecond) * dt)));
		if (m_rotationSpeed * stepsUntilRotationGoal <= Math::Abs(m_degreesPerSecond)) willOvershoot = true;
	}
	if (Math::Abs(rotationGoal) > 0.1 && !willOvershoot)
	{
		if (directionToTarget.Angle() < m_transform.rotation)
		{
			FireStarboardThruster();
		}
		else
		{
			FirePortThruster();
		}
	}
	else if (willOvershoot)
	{
		if (m_degreesPerSecond > 0) {
			FireStarboardThruster();
		}
		else
		{
			FirePortThruster();
		}
	}
}

void Enemy::OnCollision(Actor* collider)
{
	if (collider->GetTag() == "Allied Bullet" || collider->GetTag() == "Player Bullet")
	{
		//m_scene->GetGame()->AddPoints(100);
		m_velocity += ((collider->GetVelocity() - m_velocity) * .1);
		m_health -= 1;
		if (m_movementState != MovementState::DEAD)
		{
			if (collider->GetTag() == "Player Bullet")
			{
				lastDamagedByPlayer = true;
				m_scene->GetGame()->AddPoints(3);
				g_engine.GetAudio().PlaySound("enemy_hit.wav");
			}
			else lastDamagedByPlayer = false;
		}
	}
	if (collider->GetTag() == "Player" || collider->GetTag() == "Enemy" || collider->GetTag() == "Ally")
	{
		if (m_collisionCooldown < 0) {
			m_collisionCooldown = 0.1;
			Vector2 directionToCollider = (collider->GetTransform().translation - m_transform.translation).Normalized();
			Vector2 relativeVelocity = collider->GetVelocity() - m_velocity;

			float magnitude = (relativeVelocity.x * directionToCollider.x) + (relativeVelocity.y * directionToCollider.y)
				/ Math::Sqrt((directionToCollider.x * directionToCollider.x) + (directionToCollider.y * directionToCollider.y));

			m_velocity += (directionToCollider) * magnitude;
			m_health -= int(relativeVelocity.Length() / 200);

			if (m_movementState != MovementState::DEAD)
			{
				if (collider->GetTag() == "Player")
				{
					lastDamagedByPlayer = true;
					m_scene->GetGame()->AddPoints(int(relativeVelocity.Length() / 200));
					g_engine.GetAudio().PlaySound("enemy_hit.wav");
				}
				else lastDamagedByPlayer = false;
			}
		}
	}
}

void Enemy::HitByRay(std::string rayTag)
{
	if (rayTag == "Player Laser" || rayTag == "Allied Laser")
	{
		if (m_collisionCooldown < 0) {
			m_collisionCooldown = 0.1;
			m_health -= 1;
			if (m_movementState != MovementState::DEAD)
			{
				if (rayTag == "Player Laser")
				{
					lastDamagedByPlayer = true;
					m_scene->GetGame()->AddPoints(1);
					g_engine.GetAudio().PlaySound("enemy_hit.wav");
				}
				else lastDamagedByPlayer = false;
			}
		}
	}
}

void Enemy::OnDeath()
{
	if (lastDamagedByPlayer) m_scene->GetGame()->AddPoints(10);

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
			Color{0.25f, 0.0f, 0.0f},
			randomf(10.0f * m_transform.scale, 20.0f * m_transform.scale)
		};
		g_engine.GetParticleSystem().AddParticle(data);
	}
	m_destroyed = true;
}
