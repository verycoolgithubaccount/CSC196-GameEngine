#pragma once
#include "Actor.h"

class Player : public Actor
{
private:
	float m_speed = 0.03f;
	float m_rotationSpeed = 0.03f;
	float m_degreesPerSecond = 0;
public:
	Player() = default;
	Player(const Transform& transform) : Actor{ transform } {}
	Player(const Transform& transform, Model* model) : Actor{ transform, model } {}
	Player(float speed, float rotationSpeed, const Transform& transform, Model* model) : 
		Actor{ transform, model },
		m_speed{ speed },
		m_rotationSpeed{ rotationSpeed }
	{}

	void Update(float dt);
};