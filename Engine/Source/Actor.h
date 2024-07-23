#pragma once
#include "Transform.h"
#include "Particle.h"
#include "Model.h"
#include <vector>
#include <string>

class Renderer;
class Scene;

class Actor
{
protected:
	std::string m_tag;
	bool m_destroyed = false;
	float m_lifespan = 0;
	float m_collisionCooldown = 0;

	Transform m_transform;
	Vector2 m_velocity{ 0, 0 };
	float m_damping{ 0 }; // drag, velocity reduction

	Scene* m_scene{ nullptr };
	Model* m_model{ nullptr }; 
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
	Actor(const Vector2& velocity, const Transform& transform, Model* model) :
		m_velocity{ velocity },
		m_transform{ transform },
		m_model{ model }
	{}

	const Transform& GetTransform() { return m_transform; }
	const Vector2& GetVelocity() { return m_velocity; }

	virtual void Update(float dt);
	virtual void Draw(Renderer& renderer);

	void SetDamping(float damping) { m_damping = damping; }
	void SetLifeSpan(float lifespan) { m_lifespan = lifespan; }

	void SetTag(const std::string& tag) { m_tag = tag; }
	const std::string& GetTag() { return m_tag; }

	virtual void OnCollision(Actor* collider) = 0;
	float GetRadius(){ return (m_model) ? m_model->GetRadius(m_transform.scale) : 0; }

	friend class Scene; // Allow Scene to see private and protected variables
};