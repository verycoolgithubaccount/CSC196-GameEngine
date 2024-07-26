#pragma once

#include "Renderer.h"
#include "Input.h"
#include "Audio.h"

#include "EngineTime.h"

#include "Vector2.h"
#include "Random.h"
#include "MathUtils.h"

#include "Particle.h"
#include "ParticleSystem.h"

#include "Model.h"
#include "Transform.h"

#include <fmod.hpp>
#include <SDL.h>
#include <memory>

#define RENDERER g_engine.GetRenderer()

class Engine
{
private:
	bool quit{ false };

	// unique_ptr<Renderer> m_renderer instead of Renderer* m_renderer{nullptr}, it's a regular pointer but it's automatically deleted when out of scope, 
	// better than new/delete
	std::unique_ptr<Renderer> m_renderer;
	std::unique_ptr<Input> m_input;
	std::unique_ptr<Audio> m_audio;
		 
	std::unique_ptr<Time> m_time;

	std::unique_ptr<ParticleSystem> m_particleSystem;

public:

	Engine() = default;
	~Engine() = default;

	bool Initialize();
	void Shutdown();

	void Update();

	Renderer& GetRenderer() { return *m_renderer; }
	Input& GetInput() { return *m_input; }
	Audio& GetAudio() { return *m_audio; }

	ParticleSystem& GetParticleSystem() { return *m_particleSystem; }
	Time& GetTime() { return *m_time; }

	bool GameQuit() { return quit; }
};

extern Engine g_engine;

