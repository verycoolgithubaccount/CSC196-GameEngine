#include "Renderer.h"
#include "Vector2.h"
#include "Input.h"
#include "Particle.h"
#include "Random.h"
#include "EngineTime.h"

#include <SDL.h>
#include <iostream>
#include <vector>
#include <fmod.hpp>

int main(int argc, char* argv[])
{
	// Create systems
	Renderer renderer; 
	renderer.Initialize();
	renderer.CreateWindow("Game Engine", 800, 600);

	Input input;
	input.Initialize();

	// create audio system
	FMOD::System* audio;
	FMOD::System_Create(&audio);

	void* extradriverdata = nullptr;

	audio->init(32, FMOD_INIT_NORMAL, extradriverdata);

	Time time;

	vector<Vector2> points;

	vector<Particle> particles;

	/*
	for (int i = 0; i < 1000; i++)
	{
		particles.push_back(Particle{ { rand() % 800, rand() % 600}, { randomf(0, 300), 0.0f}}); // Particle at the beginning is optional like how the Vector2s I removed are optional
	}
	*/

	FMOD::Sound* sound = nullptr; 
	audio->createSound("test.wav", FMOD_DEFAULT, 0, &sound);

	audio->playSound(sound, 0, false, nullptr);

	vector<FMOD::Sound*> sounds;
	audio->createSound("bass.wav", FMOD_DEFAULT, 0, &sound);
	sounds.push_back(sound);

	audio->createSound("clap.wav", FMOD_DEFAULT, 0, &sound);
	sounds.push_back(sound);

	audio->createSound("close-hat.wav", FMOD_DEFAULT, 0, &sound);
	sounds.push_back(sound);

	audio->createSound("cowbell.wav", FMOD_DEFAULT, 0, &sound);
	sounds.push_back(sound);

	audio->createSound("open-hat.wav", FMOD_DEFAULT, 0, &sound);
	sounds.push_back(sound);

	audio->createSound("snare.wav", FMOD_DEFAULT, 0, &sound);
	sounds.push_back(sound);

	// Main loop (every frame)
	bool quit = false;
	while (!quit)
	{
		time.Tick();

		/* ---------- INPUT ---------- */

		input.Update();

		if (input.getKeyDown(SDL_SCANCODE_ESCAPE))
		{
			quit = true;
		}

		if (input.getKeyDown(SDL_SCANCODE_Q) && !input.getPreviousKeyDown(SDL_SCANCODE_Q)) audio->playSound(sounds[0], 0, false, nullptr);
		if (input.getKeyDown(SDL_SCANCODE_W) && !input.getPreviousKeyDown(SDL_SCANCODE_W)) audio->playSound(sounds[1], 0, false, nullptr);
		if (input.getKeyDown(SDL_SCANCODE_E) && !input.getPreviousKeyDown(SDL_SCANCODE_E)) audio->playSound(sounds[2], 0, false, nullptr);
		if (input.getKeyDown(SDL_SCANCODE_R) && !input.getPreviousKeyDown(SDL_SCANCODE_R)) audio->playSound(sounds[3], 0, false, nullptr);
		if (input.getKeyDown(SDL_SCANCODE_T) && !input.getPreviousKeyDown(SDL_SCANCODE_T)) audio->playSound(sounds[4], 0, false, nullptr);
		if (input.getKeyDown(SDL_SCANCODE_Y) && !input.getPreviousKeyDown(SDL_SCANCODE_Y)) audio->playSound(sounds[5], 0, false, nullptr);


		/* ---------- UPDATE ---------- */
		audio->update();

		Vector2 mousePosition = input.GetMousePosition();
		if (input.GetMouseButtonDown(0) && !input.GetPreviousMouseButtonDown(0))
		{
			for (int i = 0; i < 2; i++) {
				uint8_t color[] = {(uint8_t) random(0, 256), (uint8_t) random(0, 256), (uint8_t) random(0, 256), (uint8_t) random(0, 256) };
				for (int j = 0; j < 100; j++)
				{
					particles.push_back(Particle{ mousePosition, { randomf(-300, 300), randomf(-300, 300)}, randomf(1, 5), color });
				}
			}	
		}

		for (Particle& particle : particles)
		{
			particle.Update(time.GetDeltaTime());
			//if (particle.position.x > 800) particle.position.x = 0;
			//if (particle.position.x < 0) particle.position.x = 800;
		}

		/* ---------- DRAW ---------- */
		
		// clear screen
		renderer.SetColor(0, 0, 0, 0);
		renderer.BeginFrame();

		// draw shape
		renderer.SetColor(255, 255, 255, 0);
		for (Particle particle : particles)
		{
			particle.Draw(renderer);
		}

		for (int i = 0; points.size() > 1 && i < points.size() - 1; i++) 
		{
			renderer.SetColor(rand() % 256, rand() % 256, rand() % 256, 0);
			renderer.DrawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
		}

		// show screen
		renderer.EndFrame();
	}

	return 0;
}