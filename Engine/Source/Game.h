#pragma once

class Engine;
class Renderer;
class Scene;

class Game
{
protected:
	int m_score = 0;
	int m_lives = 0;
	int m_health = 0;
	Engine* m_engine{ nullptr };
	Scene* m_scene{ nullptr };
public:
	Game() = default;
	Game(Engine* engine) : m_engine{ engine } {}

	virtual bool Initialize() = 0;
	virtual void Shutdown() = 0;
	virtual void OnPlayerDeath() = 0;
	virtual void OnPlayerDamage(int damage) = 0;

	virtual void Update(float dt) = 0;
	virtual void Draw(Renderer& renderer) = 0;

	int GetScore() const { return m_score; }
	void AddPoints(int points) { m_score += points; }

	int GetLives() const { return m_lives; }
	void SetLives(int lives) { m_lives = lives; }

	int GetHealth() const { return m_health; }
	void SetHealth(int health) { m_health = health; }
};