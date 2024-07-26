#pragma once
#include "Particle.h"
#include "Actor.h"
#include "Vector2.h"
#include <list>
#include <vector>
#include <memory>
#include <cmath>

class Renderer;
class Game;

class Scene
{
protected:
	std::list<std::unique_ptr<Actor>> m_actors;
	std::vector<Particle> m_stars;

	Game* m_game{ nullptr };
	float m_musicTimer = 0;
public:
	//Scene() = default;
	Scene(Game* game) : m_game{ game } {}
	
	void Update(float dt);
	void Draw(Renderer& renderer);

	void AddActor(std::unique_ptr<Actor> actor);
	void RemoveAll();

	void AddStars();

	bool HasStars() { return !m_stars.empty(); }

	template<typename T>
	T* GetActor();

	bool CheckHitByRay(Vector2 originPosition, Vector2 position, std::string rayTag);

	const Vector2 GetNearestEnemyPosition(Vector2 position);
	const Vector2 GetNearestEnemyVelocity(Vector2 position);
	
	const Vector2 GetNearestAlliedPosition(Vector2 position);
	const Vector2 GetNearestAlliedVelocity(Vector2 position);

	Game* GetGame() { return m_game; }
};

template<typename T> // Template functions HAVE to be in the header
T* Scene::GetActor()
{
	for (auto& actor : m_actors)
	{
		T* result = dynamic_cast<T*>(actor.get()); // Cast to type T if possible
		if (result) return result;

		// Basically, what this does is when this function is called you need to specify the type you're looking for in <>,
		// and it searches through m_actors until it finds something of that type and returns it
	}

	return nullptr;
}