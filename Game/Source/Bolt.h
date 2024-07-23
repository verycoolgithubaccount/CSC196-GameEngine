#pragma once
#include "Actor.h"

class Bolt : public Actor
{
private:
	float m_rotationSpeed = 0.03f;
	float m_degreesPerSecond = 0;
public:
	Bolt() = default;
	Bolt(const Transform& transform) : Actor{ transform } {}
	Bolt(const Transform& transform, Model* model) : Actor{ transform, model } {}
	Bolt(float speed, Vector2 velocity, const Transform& transform, Model* model) :
		Actor{ velocity + Vector2{ speed, 0.0f }.Rotate(transform.rotation), transform, model}
	{}

	void Update(float dt);

	virtual void OnCollision(Actor* collider);
};