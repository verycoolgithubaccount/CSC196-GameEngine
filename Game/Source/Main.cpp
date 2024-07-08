#include "Renderer.h"
#include "Vector2.h"
#include "Input.h"
#include "Particle.h"
#include "Random.h"
#include "EngineTime.h"

#include <SDL.h>
#include <iostream>
#include <vector>

int main(int argc, char* argv[])
{
	// Create systems
	Renderer renderer; 
	renderer.Initialize();
	renderer.CreateWindow("Game Engine", 800, 600);

	Input input;
	input.Initialize();

	Time time;

	vector<Vector2> points;

	vector<Particle> particles;
	/*
	for (int i = 0; i < 1000; i++)
	{
		particles.push_back(Particle{ { rand() % 800, rand() % 600}, { randomf(0, 300), 0.0f}}); // Particle at the beginning is optional like how the Vector2s I removed are optional
	}
	*/

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

		/* ---------- UPDATE ---------- */

		Vector2 mousePosition = input.GetMousePosition();
		if (input.GetMouseButtonDown(0) && !input.GetPreviousMouseButtonDown(0))
		{
			for (int i = 0; i < 2; i++) {
				uint8_t color[] = { random(0, 256), random(0, 256), random(0, 256), random(0, 256) };
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