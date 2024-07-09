#include "Renderer.h"
#include "Vector2.h"
#include "Input.h"
#include "Particle.h"
#include "Random.h"
#include "EngineTime.h"
#include "MathUtils.h"
#include "Model.h"

#include <SDL.h>
#include <iostream>
#include <vector>
#include <fmod.hpp>

using namespace std;

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

	vector<Particle> particles;

	float offset = 0;

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

	vector<Vector2> points;
	points.push_back(Vector2{ - 5, 5 });
	points.push_back(Vector2{ 0, -5 });
	points.push_back(Vector2{ 5, 5 });
	points.push_back(Vector2{ - 5, 5 });
	Model model{ points, Color{ 1, 1, 1, 0 } };
	Vector2 position{ 400, 300 };
	float rotation = 0;

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

		Vector2 velocity{ 0, 0 };
		if (input.getKeyDown(SDL_SCANCODE_LEFT)) velocity.x = -100;
		if (input.getKeyDown(SDL_SCANCODE_RIGHT)) velocity.x = 100;

		if (input.getKeyDown(SDL_SCANCODE_UP)) velocity.y = -100;
		if (input.getKeyDown(SDL_SCANCODE_DOWN)) velocity.y = 100;

		position = position + velocity * time.GetDeltaTime();
		rotation = velocity.Angle(); // rotation + time.GetDeltaTime();

		/* ---------- UPDATE ---------- */
		audio->update();

		Vector2 mousePosition = input.GetMousePosition();
		if (input.GetMouseButtonDown(0) && !input.GetPreviousMouseButtonDown(0))
		{
			int colors = random(1, 5);
			for (int i = 0; i < colors; i++) {
				uint8_t color[] = {(uint8_t) random(0, 256), (uint8_t) random(0, 256), (uint8_t) random(0, 256), 0 };
				for (int j = 0; j < (200 / colors); j++)
				{
					particles.push_back(Particle{ mousePosition, randomOnUnitCircle() * randomf(0, 300), randomf(1, 5), color, randomf(2, 6) });
				}
			}	
		}

		if (input.GetMouseButtonDown(2) && input.GetPreviousMouseButtonDown(2))
		{
			for (int i = 0; i < 2; i++) {
				uint8_t color[] = { (uint8_t)random(0, 256), (uint8_t)random(0, 256), (uint8_t)random(0, 256), 0 };
				for (int j = 0; j < 5; j++)
				{
					particles.push_back(Particle{ mousePosition, randomOnUnitCircle() * randomf(0, 300), randomf(1, 5), color, randomf(2, 6) });
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

		float radius = 200;

		offset += (90 * time.GetDeltaTime());
		for (float angle = 0; angle < 360; angle += 360 / 30)
		{
			float x = Math::Cos(Math::DegToRad(angle + offset)) * Math::Sin((offset + angle) * 0.01f) * radius;
			float y = Math::Sin(Math::DegToRad(angle + offset)) * Math::Sin((offset + angle) * 0.01f) * radius;

			renderer.SetColor(random(0, 256), random(0, 256), random(0, 256), 0);
			//renderer.DrawRect(x + 400, y + 300, 10.0f, 10.0f);
		}

		// draw things
		renderer.SetColor(255, 255, 255, 0);
		model.Draw(renderer, position, rotation, 5);

		for (Particle particle : particles)
		{
			particle.Draw(renderer);
		}

		// show screen
		renderer.EndFrame();
	}

	return 0;
}