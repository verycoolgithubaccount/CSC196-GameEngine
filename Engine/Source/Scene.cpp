#include "Scene.h"
#include "Actor.h"
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
	for (Actor* actor : m_actors)
	{
		actor->Update(dt);
	}

	// destroy
	auto iter = m_actors.begin(); // "auto" automatically knows the type is std::list<Actor*>::iterator
	while (iter != m_actors.end()) 
	{
		iter = ((*iter)->m_destroyed) ? m_actors.erase(iter) : ++iter; // It has to be ++iter and not iter++ because of something to do with when it adds the 1
	}

	// An alternative to above ^ 
	//m_actors.erase(std::remove_if(m_actors.begin(), m_actors.end(), [](Actor* actor) { return actor->m_destroyed; }), m_actors.end());
	// Starting at begin until end, remove_if iterates through and adds actor to an array if it's destroyed, then erase deletes it
	std::erase_if(m_actors, [](Actor* actor) { return actor->m_destroyed; }); // Same as above^

	// collision
	for (Actor* actor1 : m_actors)
	{
		for (Actor* actor2 : m_actors)
		{
			if (actor1 == actor2) continue;

			Vector2 direction = actor1->GetTransform().translation - actor2->GetTransform().translation;
			float distance = direction.Length();
			float radius = actor1->m_model->GetRadius(actor1->GetTransform().scale) + actor2->m_model->GetRadius(actor2->GetTransform().scale);

			if (distance <= radius * 0.6)
			{
				actor1->OnCollision(actor2);
				actor2->OnCollision(actor1);
			}
		}
	}
}

void Scene::Draw(Renderer& renderer)
{
	for (Particle star : m_stars)
	{
		bool canDraw = true;
		for (Actor* actor : m_actors)
		{
			if (actor->GetTransform().translation.Distance(star.position) < (8 * actor->GetTransform().scale)) {
				canDraw = false;
			}
		}
		if (canDraw) star.Draw(renderer);
	}

	for (Actor* actor : m_actors)
	{
		actor->Draw(renderer);
	}
}

void Scene::AddActor(Actor* actor)
{
	actor->m_scene = this;
	m_actors.push_back(actor);
}

void Scene::AddStars()
{
	for (int i = 0; i < 400; i++)
	{
		float modifier = randomf();
		m_stars.push_back(Particle{
			Vector2{ randomf((float) g_engine.GetRenderer().GetWidth()), randomf((float) g_engine.GetRenderer().GetHeight()) },
			Vector2{ modifier * 10, 0.0f},
			-42.0f,
			Color{ (float) (0.5f * modifier * randomf(0.8f, 1.0f)), (float)(0.5f * modifier * randomf(0.8f, 1.0f)), (float)(0.5f * modifier * randomf(0.8f, 1.0f)), 0 },
			modifier * 0.3f
			});
	}
}
