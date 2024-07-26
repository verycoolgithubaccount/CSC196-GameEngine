#include "Bolt.h"
#include "Random.h"
#include "Engine.h"

void Bolt::Update(float dt)
{
	Actor::Update(dt);
	if (m_transform.translation.x < 0 || m_transform.translation.x > g_engine.GetRenderer().GetWidth() || m_transform.translation.y < 0 || m_transform.translation.y > g_engine.GetRenderer().GetHeight()) m_destroyed = true;
}

void Bolt::OnCollision(Actor* collider)
{
	if (collider->GetTag() != this->GetTag()) OnDeath();
}

void Bolt::HitByRay(std::string rayTag)
{
	OnDeath();
}

void Bolt::OnDeath()
{
	m_destroyed = true;
}
