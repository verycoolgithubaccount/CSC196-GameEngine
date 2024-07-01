#include "Renderer.h"

#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[])
{
	Renderer renderer; 
	renderer.Initialize();
	renderer.CreateWindow("Game Engine", 800, 600);

	while (true)
	{
		//// clear screen
		renderer.SetColor(0, 0, 0, 0);
		renderer.BeginFrame();

		//// draw cube
		renderer.SetColor(255, 255, 255, 0);
		renderer.DrawLine(200, 100, 600, 100);
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
		renderer.DrawLine(600, 500, 530, 430);

		for (int i = 0; i < 300; i++) {
			renderer.SetColor(rand() % 256, rand() % 256, rand() % 256, 0); // Random color
			renderer.DrawLine(rand() % 800, rand() % 600, rand() % 800, rand() % 600); // Random line
			renderer.DrawPoint(rand() % 800, rand() % 600); // Random point
		}

		//// show screen
		renderer.EndFrame();
	}

	return 0;
}