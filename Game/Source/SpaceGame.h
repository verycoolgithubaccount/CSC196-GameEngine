#pragma once
#include "Game.h"
#include "Font.h"
#include "Text.h"
#include "Particle.h"
#include <vector>

class SpaceGame : public Game
{
public:
	// putting char means it'll store it in as many bytes as a char (128 enum spaces), default is an int which is way bigger (this is not necessary)
	// "enum class" instead of "enum" makes it scoped, so you have to do State::TITLE instead of just TITLE
	enum class State : char 
	{
		TITLE,
		START_GAME,
		START_LEVEL,
		GAME,
		PLAYER_DEAD,
		GAME_OVER
	};

private:
	State m_state{ State::TITLE };
	float m_spawnTimer{ 0 };
	float m_spawnTime{ 0 };
	float m_stateTimer{ 0 };
	float m_textTimer{ 0 };
	float m_healthWarningTimer{ 0 };

	float m_laserEnergy = 0;
	const float MAX_LASER_ENERGY = 15;

	std::vector<Particle> m_livesDisplay;
	std::vector<Particle> m_healthDisplay;
	std::vector<Particle> m_laserEnergyDisplay;

	Font* m_avignon = new Font();
	Font* m_avignonSmall = new Font();
	Font* m_avignonItalic = new Font();
	Font* m_homeworld = new Font();
	Font* m_begok = new Font();

	Text* m_titleText = new Text(m_avignonItalic);
	Text* m_subtitleText = new Text(m_homeworld);
	Text* m_startText = new Text(m_avignon);
	Text* m_livesText = new Text(m_avignonSmall);
	Text* m_healthText = new Text(m_avignonSmall);
	Text* m_scoreText = new Text(m_begok);
	Text* m_laserEnergyText = new Text(m_avignonSmall);
public:
	SpaceGame(Engine* engine) : Game{ engine } {}

	bool Initialize() override;
	void Shutdown() override;
	void Update(float dt) override;
	void Draw(Renderer& renderer) override;

	void OnPlayerDeath() override;
	void OnPlayerDamage(int damage) override;

	void AddLaserEnergy(float modifier);

	float GetMaxLaserEnergy() const { return MAX_LASER_ENERGY; }
	float GetLaserEnergy() const { return m_laserEnergy; }
};