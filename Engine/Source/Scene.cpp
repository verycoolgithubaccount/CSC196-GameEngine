#include "Scene.h"
#include "Actor.h"

void Scene::Update(float dt)
{
	for (Actor* actor : m_actors)
	{
		actor->Update(dt);
	}
}

void Scene::Draw(Renderer& renderer)
{
	for (Actor* actor : m_actors)
	{
		actor->Draw(renderer);
	}
}

void Scene::AddActor(Actor* actor)
{
	m_actors.push_back(actor);
}
