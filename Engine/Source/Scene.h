#pragma once
#include <list>

class Renderer;
class Actor;

class Scene
{
protected:
	std::list<Actor*> m_actors;

public:
	Scene() = default;
	
	void Update(float dt);
	void Draw(Renderer& renderer);

	void AddActor(Actor* actor);
};