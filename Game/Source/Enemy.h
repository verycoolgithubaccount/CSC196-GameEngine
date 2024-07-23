#pragma once
#include "Actor.h"

class Enemy : public Actor
{
public:
	enum class MovementState : char
	{
		SENTRY = 0,
		FOLLOWING,
		TRAVELLING
	};
private:
	MovementState m_movementState{ MovementState::TRAVELLING };
	Vector2 m_lastTargetVelocity{ 0, 0 };
	float m_speed = 0.03f;
	float m_rotationSpeed = 0.03f;
	float m_degreesPerSecond = 0;
	float m_fireTimer = 0;
	int m_health = 3;
public:
	Enemy() = default;
	Enemy(const Transform& transform) : Actor{ transform } {}
	Enemy(const Transform& transform, Model* model) : Actor{ transform, model } {}
	Enemy(float speed, float rotationSpeed, const Transform& transform, Model* model) :
		Actor{ transform, model },
		m_speed{ speed },
		m_rotationSpeed{ rotationSpeed }
	{}

	void Update(float dt);
	void FireStarboardThruster();
	void FirePortThruster();
	void FireMainThruster(float dt);
	void Rotate(Vector2 directionToTarget, float rotationGoal, float dt);

	void OnCollision(Actor* collider);
};