#include "Bolt.h"

void Bolt::Update(float dt)
{
	Actor::Update(dt);
}

void Bolt::OnCollision(Actor* collider)
{
	if (collider->GetTag() != this->GetTag())
	{
		m_destroyed = true;
	}
}
