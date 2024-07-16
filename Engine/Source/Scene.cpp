#include "Scene.h"
#include "Actor.h"
#include "Engine.h"

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
}

void Scene::Draw(Renderer& renderer)
{
	for (Particle star : m_stars)
	{
		bool canDraw = true;
		for (Actor* actor : m_actors)
		{
			if (actor->GetPosition().Distance(star.position) < (8 * actor->GetSize())) {
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
			Color{ (float) (0.5 * modifier * randomf(0.8, 1)), (float)(0.5 * modifier * randomf(0.8, 1)), (float)(0.5 * modifier * randomf(0.8, 1)), 0 },
			modifier * 0.3f
			});
	}
}
