#pragma once
#include "Game.h"

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

public:
	SpaceGame(Engine* engine) : Game{ engine } {}

	bool Initialize() override;
	void Shutdown() override;
	void Update(float dt) override;
	void Draw(Renderer& renderer) override;

	void OnPlayerDeath() override;
};