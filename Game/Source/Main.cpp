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
	g_engine.GetAudio().AddSound("bolt_fire.wav");
	g_engine.GetAudio().AddSound("boom.wav");
	g_engine.GetAudio().AddSound("ship_collision.wav");

	while (!g_engine.GameQuit())
	{
		g_engine.Update();
		game->Update(g_engine.GetTime().GetDeltaTime());

		g_engine.GetRenderer().SetColor(0, 0, 0, 0);
		g_engine.GetRenderer().BeginFrame();

		game->Draw(g_engine.GetRenderer());

		g_engine.GetRenderer().EndFrame();
	}

	return 0;
}