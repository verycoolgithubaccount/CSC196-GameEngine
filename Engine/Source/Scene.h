#pragma once
#include "Particle.h"
#include <list>
#include <vector>

class Renderer;
class Actor;

class Scene
{
protected:
	std::list<Actor*> m_actors;
	std::vector<Particle> m_stars;

public:
	Scene() = default;
	
	void Update(float dt);
	void Draw(Renderer& renderer);

	void AddActor(Actor* actor);

	void AddStars();
};