#include "Engine.h"

Engine g_engine;

bool Engine::Initialize()
{
    m_renderer = new Renderer();
    m_input = new Input();
    m_audio = new Audio();

    m_renderer->Initialize();
    m_renderer->CreateWindow("Game Engine", 1500, 800);
    m_input->Initialize();
    m_audio->Initialize();

    return true;
}

void Engine::Shutdown()
{
    m_renderer->Shutdown();
    m_input->Shutdown();
    m_audio->Shutdown();
}

void Engine::Update()
{
    m_input->Update();
    m_audio->Update();
}
