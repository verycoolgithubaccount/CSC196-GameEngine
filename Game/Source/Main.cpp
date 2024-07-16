#include "Vector2.h"
#include "Particle.h"
#include "Random.h"
#include "EngineTime.h"
#include "MathUtils.h"
#include "Model.h"
#include "Transform.h"
#include "Player.h"
#include "Scene.h"
#include "Engine.h"

#include <SDL.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <fmod.hpp>

using namespace std;

int main(int argc, char* argv[])
{
	// Create systems
	g_engine.Initialize();

	Time time;

	vector<Particle> particles;

	float offset = 0;

	g_engine.GetAudio().AddSound("test.wav");
	g_engine.GetAudio().AddSound("bass.wav");
	g_engine.GetAudio().AddSound("clap.wav");
	g_engine.GetAudio().AddSound("close-hat.wav");
	g_engine.GetAudio().AddSound("cowbell.wav");
	g_engine.GetAudio().AddSound("open-hat.wav");
	g_engine.GetAudio().AddSound("snare.wav");

	// Make a ship
	vector<Vector2> model_points;

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

	// actor
	Model* model = new Model{ model_points, Color{ 0, 0.5, 0.5 } };
	Transform transform{ { g_engine.GetRenderer().GetWidth() >> 1, g_engine.GetRenderer().GetHeight() >> 1 }, 0, 5 }; // >> 1 is bitshift right, exactly the same as /2 but it looks fancier

	Player* player = new Player(0.03f, 0.03f, transform, model);
	player->SetDamping(0); // There's no air in space

	Scene* scene = new Scene();
	scene->AddActor(player);



	// Main loop (every frame)
	bool quit = false;
	while (!quit)
	{
		time.Tick();

		/* ---------- INPUT ---------- */

		g_engine.Update();

		if (g_engine.GetInput().getKeyDown(SDL_SCANCODE_ESCAPE))
		{
			quit = true;
		}

		/* ---------- UPDATE ---------- */
		scene->Update(time.GetDeltaTime());

		Vector2 mousePosition = g_engine.GetInput().GetMousePosition();
		if (g_engine.GetInput().GetMouseButtonDown(0) && !g_engine.GetInput().GetPreviousMouseButtonDown(0))
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

		if (g_engine.GetInput().GetMouseButtonDown(2) && g_engine.GetInput().GetPreviousMouseButtonDown(2))
		{
			for (int i = 0; i < 2; i++) {
				Color color{ randomf(), randomf(), randomf(), 0 };
				for (int j = 0; j < 5; j++)
				{
					particles.push_back(Particle{ mousePosition, randomOnUnitCircle() * randomf(0, 300), randomf(1, 5), color, randomf(2, 6) });
				}
			}
		}

		for (Particle& particle : particles)
		{
			particle.Update(time.GetDeltaTime());
			if (particle.position.x > g_engine.GetRenderer().GetWidth()) particle.position.x = 0;
			if (particle.position.x < 0) particle.position.x = (float) g_engine.GetRenderer().GetWidth();
			if (particle.position.y > g_engine.GetRenderer().GetHeight()) particle.position.y = 0;
			if (particle.position.y < 0) particle.position.y = (float) g_engine.GetRenderer().GetHeight();
		}

		/* ---------- DRAW ---------- */
		
		// clear screen
		g_engine.GetRenderer().SetColor(0, 0, 0, 0);
		g_engine.GetRenderer().BeginFrame();

		float radius = 200;

		offset += (90 * time.GetDeltaTime());
		for (float angle = 0; angle < 360; angle += 360 / 30)
		{
			float x = Math::Cos(Math::DegToRad(angle + offset)) * Math::Sin((offset + angle) * 0.01f) * radius;
			float y = Math::Sin(Math::DegToRad(angle + offset)) * Math::Sin((offset + angle) * 0.01f) * radius;

			g_engine.GetRenderer().SetColor(random(0, 256), random(0, 256), random(0, 256), 0);
		}

		// draw things

		for (Particle particle : particles)
		{
			particle.Draw(g_engine.GetRenderer());
		}

		scene->Draw(g_engine.GetRenderer());

		// show screen
		g_engine.GetRenderer().EndFrame();
	}

	return 0;
}