#include "Engine.h"
#include "SpaceGame.h"
#include "Font.h"
#include "Text.h"


int main(int argc, char* argv[])
{
	g_engine.Initialize();

	SpaceGame* game = new SpaceGame(&g_engine);
	game->Initialize();

	g_engine.GetAudio().AddSound("main_thruster_start.wav");
	g_engine.GetAudio().AddSound("main_thruster_ambient.wav");
	g_engine.GetAudio().AddSound("main_thruster_stop.wav");
	g_engine.GetAudio().AddSound("music.wav");

	Font* font = new Font();
	font->Load("Avignon.ttf", 20);

	Text* text = new Text(font);
	text->Create(g_engine.GetRenderer(), "Text", Color{ 1, 1, 1 });

	while (!g_engine.GameQuit())
	{
		g_engine.Update();
		game->Update(g_engine.GetTime().GetDeltaTime());

		g_engine.GetRenderer().SetColor(0, 0, 0, 0);
		g_engine.GetRenderer().BeginFrame();

		game->Draw(g_engine.GetRenderer());
		text->Draw(g_engine.GetRenderer(), 40, 40);

		g_engine.GetRenderer().EndFrame();
	}

	return 0;
}