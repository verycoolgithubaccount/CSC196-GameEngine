#include "Pickup.h"
#include "Player.h"

void Pickup::OnCollision(Actor* collider)
{
	if (collider->GetTag() == "Player")
	{
		dynamic_cast<Player*>(collider)->SetFireModifier(1);
		m_destroyed = true;
	}
}
