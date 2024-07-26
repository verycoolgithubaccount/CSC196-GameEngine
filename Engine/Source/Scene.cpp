#include "Scene.h"
#include "Engine.h"
#include "Model.h"
#include <algorithm>

void Scene::Update(float dt)
{
	for (Particle& star : m_stars)
	{
		star.Update(dt);

		star.position.x = Math::Wrap(star.position.x, (float)g_engine.GetRenderer().GetWidth());
		star.position.y = Math::Wrap(star.position.y, (float)g_engine.GetRenderer().GetHeight());
	}
	for (auto& actor : m_actors)
	{
		actor->Update(dt);
	}

	m_musicTimer -= dt;
	if (m_musicTimer <= 0) {
		g_engine.GetAudio().PlaySound("music.wav");
		m_musicTimer = 275;
	}

	// destroy
	//auto iter = m_actors.begin(); // "auto" automatically knows the type is std::list<Actor*>::iterator
	//while (iter != m_actors.end()) 
	//{
	//	iter = ((*iter)->m_destroyed) ? m_actors.erase(iter) : ++iter; // It has to be ++iter and not iter++ because of something to do with when it adds the 1
	//}

	// An alternative to above ^ 
	//m_actors.erase(std::remove_if(m_actors.begin(), m_actors.end(), [](Actor* actor) { return actor->m_destroyed; }), m_actors.end());
	// Starting at begin until end, remove_if iterates through and adds actor to an array if it's destroyed, then erase deletes it
	std::erase_if(m_actors, [](auto& actor) { return actor->m_destroyed; }); // Same as above^

	// collision
	for (auto& actor1 : m_actors)
	{
		for (auto& actor2 : m_actors)
		{
			if (actor1 == actor2 || (actor1->m_destroyed || actor2->m_destroyed)) continue;

			Vector2 direction = actor1->GetTransform().translation - actor2->GetTransform().translation;
			float distance = direction.Length();
			float radius = actor1->GetRadius() + actor2->GetRadius();

			if (distance <= radius * 0.6)
			{
				actor1->OnCollision(actor2.get());
				actor2->OnCollision(actor1.get());
			}
		}
	}
}

bool Scene::CheckHitByRay(Vector2 originPosition, Vector2 position, std::string rayTag)
{
	for (auto& actor : m_actors)
	{
		if (actor->m_destroyed || actor->GetTransform().translation == originPosition) continue;

		if ((actor->GetTransform().translation - position).Length() <= actor->GetRadius() * 0.6)
		{
			actor->HitByRay(rayTag);
			return true;
		}
	}
	return false;
}

void Scene::Draw(Renderer& renderer)
{
	for (Particle& star : m_stars)
	{
		bool canDraw = true;
		for (auto& actor : m_actors)
		{
			if (actor->GetTransform().translation.Distance(star.position) < (8 * actor->GetTransform().scale)) {
				canDraw = false;
			}
		}
		if (canDraw) star.Draw(renderer);
	}

	for (auto& actor : m_actors)
	{
		actor->Draw(renderer);
	}
}

void Scene::AddActor(std::unique_ptr<Actor> actor)
{
	actor->m_scene = this;
	m_actors.push_back(std::move(actor)); //std::move is to delete the original unique_ptr and use it in the list instead
}

void Scene::RemoveAll()
{
	m_actors.clear();
}

void Scene::AddStars()
{
	for (int i = 0; i < 400; i++)
	{
		float modifier = randomf();
		
		m_stars.push_back(Particle{
			Vector2{ randomf((float)g_engine.GetRenderer().GetWidth()), randomf((float)g_engine.GetRenderer().GetHeight()) },
			Vector2{ modifier * 10, 0.0f},
			-42.0f,
			Color{ (float)(0.5f * modifier * randomf(0.8f, 1.0f)), (float)(0.5f * modifier * randomf(0.8f, 1.0f)), (float)(0.5f * modifier * randomf(0.8f, 1.0f))},
			modifier * 0.3f
			});
	}
}

const Vector2 Scene::GetNearestEnemyPosition(Vector2 position)
{
	float distance = HUGE_VAL;
	for (auto& actor : m_actors)
	{
		if (actor->GetTag() == "Enemy" && distance > position.Distance(actor->GetTransform().translation)) distance = position.Distance(actor->GetTransform().translation);
	}

	if (distance == HUGE_VAL) return Vector2{ (float)HUGE_VAL, (float)HUGE_VAL };

	for (auto& actor : m_actors)
	{
		if (actor->GetTag() == "Enemy" && distance == position.Distance(actor->GetTransform().translation)) return actor->GetTransform().translation;
	}
}

const Vector2 Scene::GetNearestEnemyVelocity(Vector2 position)
{
	float distance = HUGE_VAL;
	for (auto& actor : m_actors)
	{
		if (actor->GetTag() == "Enemy" && distance > position.Distance(actor->GetTransform().translation)) distance = position.Distance(actor->GetTransform().translation);
	}

	if (distance == HUGE_VAL) return Vector2{ (float)HUGE_VAL, (float)HUGE_VAL };

	for (auto& actor : m_actors)
	{
		if (actor->GetTag() == "Enemy" && distance == position.Distance(actor->GetTransform().translation)) return actor->GetVelocity();
	}
}

const Vector2 Scene::GetNearestAlliedPosition(Vector2 position)
{
	float distance = HUGE_VAL;
	for (auto& actor : m_actors)
	{
		if ((actor->GetTag() == "Player" || actor->GetTag() == "Ally") && distance > position.Distance(actor->GetTransform().translation)) distance = position.Distance(actor->GetTransform().translation);
	}

	if (distance == HUGE_VAL) return Vector2{ (float)HUGE_VAL, (float)HUGE_VAL };

	for (auto& actor : m_actors)
	{
		if ((actor->GetTag() == "Player" || actor->GetTag() == "Ally") && distance == position.Distance(actor->GetTransform().translation)) return actor->GetTransform().translation;
	}
}

const Vector2 Scene::GetNearestAlliedVelocity(Vector2 position)
{
	float distance = HUGE_VAL;
	for (auto& actor : m_actors)
	{
		if ((actor->GetTag() == "Player" || actor->GetTag() == "Ally") && distance > position.Distance(actor->GetTransform().translation)) distance = position.Distance(actor->GetTransform().translation);
	}

	if (distance == HUGE_VAL) return Vector2{ (float)HUGE_VAL, (float)HUGE_VAL };

	for (auto& actor : m_actors)
	{
		if ((actor->GetTag() == "Player" || actor->GetTag() == "Ally") && distance == position.Distance(actor->GetTransform().translation)) return actor->GetVelocity();
	}
}