#include "Renderer.h"
using namespace std;

bool Renderer::Initialize()
{
	// initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
		return false;
	}
	return true;
}

void Renderer::Shutdown()
{
}

bool Renderer::CreateWindow(string title, int width, int height)
{
	m_width = width;
	m_height = height;
	// create window
	// returns pointer to window if successful or nullptr if failed
	m_window = SDL_CreateWindow(title.c_str(), // Get the pointer to the string array instead of the string class
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height,
		SDL_WINDOW_SHOWN);
	if (m_window == nullptr)
	{
		std::cerr << "Error creating SDL window: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return false;
	}

	// create renderer
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // | is bitwise OR, combines the binary values of these enums (assigned 2 and 4, add to get 6)
	if (m_renderer == nullptr)
	{
		std::cerr << "Error creating SDL renderer: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return false;
	}
	return true;
}

void Renderer::BeginFrame()
{
	SDL_RenderClear(m_renderer);
}

void Renderer::EndFrame()
{
	SDL_RenderPresent(m_renderer);
}

void Renderer::SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
}

void Renderer::DrawLine(int x1, int y1, int x2, int y2)
{
	SDL_RenderDrawLine(m_renderer, x1, y1, x2, y2);
}

void Renderer::DrawLine(float x1, float y1, float x2, float y2)
{
	SDL_RenderDrawLineF(m_renderer, x1, y1, x2, y2);
}

void Renderer::DrawLine(Vector2 vector1, Vector2 vector2)
{
	SDL_RenderDrawLineF(m_renderer, vector1.x, vector1.y, vector2.x, vector2.y);
}

void Renderer::DrawPoint(int x, int y)
{
	SDL_RenderDrawPoint(m_renderer, x, y);
}

void Renderer::DrawPoint(float x, float y)
{
	SDL_RenderDrawPointF(m_renderer, x, y);
}

void Renderer::DrawPoint(Vector2 vector)
{
	SDL_RenderDrawPointF(m_renderer, vector.x, vector.y);
}

void Renderer::DrawRect(int x, int y, int w, int h)
{
	SDL_Rect rect{ x - (w / 2), y - (h / 2), w, h};
	SDL_RenderFillRect(m_renderer, &rect);
}

void Renderer::DrawRect(float x, float y, float w, float h)
{
	SDL_FRect rect{ x - (w / 2), y - (h / 2), w, h};
	SDL_RenderFillRectF(m_renderer, &rect);
}
