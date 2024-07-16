#pragma once
#include "Transform.h"
#include "Particle.h"
#include <vector>

class Renderer;

class Actor
{
protected:
	bool m_destroyed = false;
	float m_lifespan = 0;

	Transform m_transform;
	Vector2 m_velocity{ 0, 0 };
	float m_damping{ 0 }; // drag, velocity reduction

	class Model* m_model{ nullptr }; 
	// ^ You can do this if you don't want to include a class / use its members but want to reference or point to it
	// Or you can put "class Model;" below the #includes and just put "Model*" here, like I did with Renderer

	std::vector<Particle> m_particles;

public:
	Actor() = default;
	Actor(const Transform& transform) : m_transform{ transform } {}
	Actor(const Transform& transform, Model* model) : 
		m_transform{ transform },
		m_model{ model }
	{}

	Vector2 GetPosition() { return m_transform.translation; }
	float GetSize() { return m_transform.scale; }

	virtual void Update(float dt);
	virtual void Draw(Renderer& renderer);

	void SetDamping(float damping) { m_damping = damping; }
};