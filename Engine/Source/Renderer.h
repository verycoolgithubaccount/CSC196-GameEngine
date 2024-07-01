#pragma once
#include <iostream>
#include <SDL.h>
using namespace std;

class Renderer 
{
private:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;

public:
	Renderer() = default; // Default constructor, can also do Renderer() {}
	bool Initialize();
	void Shutdown();

	bool CreateWindow(string title, int width, int height);
	void BeginFrame();
	void EndFrame();

	void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a); // unsigned char would work (0-255) instead of uint8_t (unsigned int 8 bit type, 0-255)
	void DrawLine(int x1, int y1, int x2, int y2);
	void DrawPoint(int x, int y);
};