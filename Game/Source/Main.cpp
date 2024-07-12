#include "Renderer.h"
#include "Vector2.h"
#include "Input.h"
#include "Particle.h"
#include "Random.h"
#include "EngineTime.h"
#include "MathUtils.h"
#include "Model.h"
#include "Transform.h"

#include <SDL.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <fmod.hpp>

using namespace std;

int main(int argc, char* argv[])
{
	// Create systems
	Renderer renderer; 
	renderer.Initialize();
	renderer.CreateWindow("Game Engine", 1100, 900);

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

	// Make a ship
	vector<Vector2> model_points;
	/*
	model_points.push_back(Vector2{ 5, 0 });
	model_points.push_back(Vector2{ -5, -5 });
	model_points.push_back(Vector2{ -5, 5 });
	model_points.push_back(Vector2{ 5, 0 });
	*/

	model_points.push_back(Vector2{0, -1});
	model_points.push_back(Vector2{3, -2});
	model_points.push_back(Vector2{6, -1});
	model_points.push_back(Vector2{6, 1});
	model_points.push_back(Vector2{3, 2});
	model_points.push_back(Vector2{0, 1});
	model_points.push_back(Vector2{0, -1});
	model_points.push_back(Vector2{-5, -1});
	model_points.push_back(Vector2{-8, -2});
	model_points.push_back(Vector2{-8, 2});
	model_points.push_back(Vector2{-5, 1});
	model_points.push_back(Vector2{-5, -3});
	model_points.push_back(Vector2{-6, -4});
	model_points.push_back(Vector2{-3, -4});
	model_points.push_back(Vector2{-4, -3});
	model_points.push_back(Vector2{-5, -3});
	model_points.push_back(Vector2{-4, -3});
	model_points.push_back(Vector2{-4, -2});
	model_points.push_back(Vector2{-5, -1});
	model_points.push_back(Vector2{-4, -2});
	model_points.push_back(Vector2{-1, -3});
	model_points.push_back(Vector2{-2, -7});
	model_points.push_back(Vector2{-3, -10});
	model_points.push_back(Vector2{3, -4});
	model_points.push_back(Vector2{5, -4});
	model_points.push_back(Vector2{5, -3});
	model_points.push_back(Vector2{-1, -3});
	model_points.push_back(Vector2{2, -3});
	model_points.push_back(Vector2{3, -4});
	model_points.push_back(Vector2{2, -3});
	model_points.push_back(Vector2{3, -3});
	model_points.push_back(Vector2{6, -1});
	model_points.push_back(Vector2{6, 1});
	model_points.push_back(Vector2{3, 3});
	model_points.push_back(Vector2{-1, 3});
	model_points.push_back(Vector2{-2, 7});
	model_points.push_back(Vector2{-3, 10});
	model_points.push_back(Vector2{3, 4});
	model_points.push_back(Vector2{5, 4});
	model_points.push_back(Vector2{5, 3});
	model_points.push_back(Vector2{2, 3});
	model_points.push_back(Vector2{3, 4});
	model_points.push_back(Vector2{2, 3});
	model_points.push_back(Vector2{-1, 3});
	model_points.push_back(Vector2{-4, 2});
	model_points.push_back(Vector2{-4, 3});
	model_points.push_back(Vector2{-3, 4});
	model_points.push_back(Vector2{-6, 4});
	model_points.push_back(Vector2{-5, 3});
	model_points.push_back(Vector2{-4, 3});
	model_points.push_back(Vector2{-5, 3});
	model_points.push_back(Vector2{-5, 1});
	model_points.push_back(Vector2{-4, 2});
	model_points.push_back(Vector2{-5, 1});
	model_points.push_back(Vector2{0, 1});

	Model model{ model_points, Color{ 0, 0.5, 0.5 } };

	Transform transform{ { renderer.GetWidth() >> 1, renderer.GetHeight() >> 1 }, 0, 5 }; // >> 1 is bitshift right, exactly the same as /2 but it looks fancier

	Vector2 velocity = Vector2{ 0.0f, 0.0f };
	float degreesPerSecond = 0;

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
		
		/*
		if (input.getKeyDown(SDL_SCANCODE_Q) && !input.getPreviousKeyDown(SDL_SCANCODE_Q)) audio->playSound(sounds[0], 0, false, nullptr);
		if (input.getKeyDown(SDL_SCANCODE_W) && !input.getPreviousKeyDown(SDL_SCANCODE_W)) audio->playSound(sounds[1], 0, false, nullptr);
		if (input.getKeyDown(SDL_SCANCODE_E) && !input.getPreviousKeyDown(SDL_SCANCODE_E)) audio->playSound(sounds[2], 0, false, nullptr);
		if (input.getKeyDown(SDL_SCANCODE_R) && !input.getPreviousKeyDown(SDL_SCANCODE_R)) audio->playSound(sounds[3], 0, false, nullptr);
		if (input.getKeyDown(SDL_SCANCODE_T) && !input.getPreviousKeyDown(SDL_SCANCODE_T)) audio->playSound(sounds[4], 0, false, nullptr);
		if (input.getKeyDown(SDL_SCANCODE_Y) && !input.getPreviousKeyDown(SDL_SCANCODE_Y)) audio->playSound(sounds[5], 0, false, nullptr);
		*/

		float thrust = 0;
		if (input.getKeyDown(SDL_SCANCODE_LEFT)) degreesPerSecond -= 0.03f;
		if (input.getKeyDown(SDL_SCANCODE_RIGHT)) degreesPerSecond += 0.03f;

		//if (input.getKeyDown(SDL_SCANCODE_DOWN)) thrust = -40; // commenting out = no reverse
		if (input.getKeyDown(SDL_SCANCODE_UP)) thrust = 0.03f;

		transform.rotation += Math::DegToRad(degreesPerSecond) * time.GetDeltaTime();
		velocity += Vector2{ thrust, 0.0f }.Rotate(transform.rotation);
		transform.translation += velocity * time.GetDeltaTime();
		transform.translation.x = Math::Wrap(transform.translation.x, (float) renderer.GetWidth());
		transform.translation.y = Math::Wrap(transform.translation.y, (float) renderer.GetHeight());
		//transform.rotation = velocity.Angle(); // rotation + time.GetDeltaTime();

		/* ---------- UPDATE ---------- */
		audio->update();

		Vector2 mousePosition = input.GetMousePosition();
		if (input.GetMouseButtonDown(0) && !input.GetPreviousMouseButtonDown(0))
		{
			int colors = random(1, 5);
			for (int i = 0; i < colors; i++) {
				Color color{ randomf(), randomf(), randomf(), 0};
				for (int j = 0; j < (200 / colors); j++)
				{
					particles.push_back(Particle{ mousePosition, randomOnUnitCircle() * randomf(0, 300), randomf(1, 5), color, randomf(2, 6) });
				}
			}	
		}

		if (input.GetMouseButtonDown(2) && input.GetPreviousMouseButtonDown(2))
		{
			for (int i = 0; i < 2; i++) {
				Color color{ randomf(), randomf(), randomf(), 0 };
				for (int j = 0; j < 5; j++)
				{
					particles.push_back(Particle{ mousePosition, randomOnUnitCircle() * randomf(0, 300), randomf(1, 5), color, randomf(2, 6) });
				}
			}
		}

		int randomChance = random(30);
		if (randomChance == 1)
		{
			if (thrust > 0)
			{
				particles.push_back(Particle{ // Add particles for main thruster
					(transform.translation - (Vector2{8.4f, 0 + randomf(-1, 1)} * transform.scale).Rotate(transform.rotation)),
					velocity + (Vector2{-thrust * 200, randomf(-1.6, 1.6)} * transform.scale).Rotate(transform.rotation),
					randomf(1, 5),
					Color{randomf(), randomf(0.8f, 1), 1, 0},
					randomf(0.6 * transform.scale, 1.6 * transform.scale)
					});
			}
			if (input.getKeyDown(SDL_SCANCODE_RIGHT))
			{
				particles.push_back(Particle{ // Add particles for port thruster
					(transform.translation - (Vector2{5.0f + randomf(-0.8, 0.8), -4.4f} * transform.scale).Rotate(transform.rotation)),
					velocity + (Vector2{randomf(-1.6, 1.6), 6.0f} * transform.scale).Rotate(transform.rotation),
					randomf(1, 3),
					Color{1, 1, 1, 0},
					randomf(0.2 * transform.scale, 0.6 * transform.scale)
					});
			}
			if (input.getKeyDown(SDL_SCANCODE_LEFT))
			{
				particles.push_back(Particle{ // Add particles for starboard thruster
					(transform.translation - (Vector2{5.0f + randomf(-0.8, 0.8), 4.4f} * transform.scale).Rotate(transform.rotation)),
					velocity + (Vector2{randomf(-1.6, 1.6), -6.0f} * transform.scale).Rotate(transform.rotation),
					randomf(1, 3),
					Color{1, 1, 1, 0},
					randomf(0.2 * transform.scale, 0.6 * transform.scale)
					});
			}
		}

		for (Particle& particle : particles)
		{
			particle.Update(time.GetDeltaTime());
			if (particle.position.x > renderer.GetWidth()) particle.position.x = 0;
			if (particle.position.x < 0) particle.position.x = (float) renderer.GetWidth();
			if (particle.position.y > renderer.GetHeight()) particle.position.y = 0;
			if (particle.position.y < 0) particle.position.y = (float) renderer.GetHeight();
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

		for (Particle particle : particles)
		{
			particle.Draw(renderer);
		}

		model.Draw(renderer, transform);

		// show screen
		renderer.EndFrame();
	}

	return 0;
}