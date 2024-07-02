#include "Renderer.h"
#include "Vector2.h"
#include "Input.h"

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

	vector<Vector2> points;
	/*for (int i = 0; i < 100; i++)
	{
		points.push_back(Vector2{ rand() % 800, rand() % 600 });
	}*/

	// Main loop (every frame)
	bool quit = false;
	while (!quit)
	{
		/* ---------- INPUT ---------- */

		input.Update();

		if (input.getKeyDown(SDL_SCANCODE_ESCAPE))
		{
			quit = true;
		}

		/* ---------- UPDATE ---------- */

		Vector2 mousePosition = input.GetMousePosition();
		//cout << mousePosition.x << ", " << mousePosition.y << endl;

		if (input.GetMouseButtonDown(0) && !input.GetPreviousMouseButtonDown(0)) // Click left button
		{
			points.push_back(mousePosition);
		}
		if (input.GetMouseButtonDown(0) && input.GetPreviousMouseButtonDown(0)) // Hold left button
		{
			float distance = (points.back() - mousePosition).Length(); // Get distance between last and current point
			if (distance > 5) points.push_back(mousePosition);
		}
		if (!input.GetMouseButtonDown(0) && input.GetPreviousMouseButtonDown(0))
		{
			cout << "Left button released" << endl;
		}

		/*Vector2 speed{ 0.1f, -0.1f };
		for (Vector2& point : points)
		{
			point = point + 0.002f;
		}*/

		/* ---------- DRAW ---------- */
		
		// clear screen
		renderer.SetColor(0, 0, 0, 0);
		renderer.BeginFrame();

		// draw cube
		renderer.SetColor(255, 255, 255, 0);
		/*renderer.DrawLine(200, 100, 600, 100);
		renderer.DrawLine(200, 500, 600, 500);
		renderer.DrawLine(200, 100, 200, 500);
		renderer.DrawLine(600, 100, 600, 500);
		
		renderer.DrawLine(270, 170, 530, 170);
		renderer.DrawLine(270, 430, 530, 430);
		renderer.DrawLine(270, 170, 270, 430);
		renderer.DrawLine(530, 170, 530, 430);
		
		renderer.DrawLine(200, 100, 270, 170);
		renderer.DrawLine(600, 100, 530, 170);
		renderer.DrawLine(200, 500, 270, 430);
		renderer.DrawLine(600, 500, 530, 430);*/

		for (int i = 0; points.size() > 1 && i < points.size() - 1; i++) 
		{
			renderer.SetColor(rand() % 256, rand() % 256, rand() % 256, 0);
			renderer.DrawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
		}

		/*for (int i = 0; i < 300; i++) 
		{
			renderer.SetColor(rand() % 256, rand() % 256, rand() % 256, 0); // Random color
			renderer.DrawLine(rand() % 800, rand() % 600, rand() % 800, rand() % 600); // Random line
			renderer.DrawPoint(rand() % 800, rand() % 600); // Random point
		}
		*/

		// show screen
		renderer.EndFrame();
	}

	return 0;
}