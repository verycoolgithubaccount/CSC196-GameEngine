#include "Engine.h"

Engine g_engine;

bool Engine::Initialize()
{
    //make_unique<Renderer>() instead of new Renderer() bc it's using a shared_ptr
    m_renderer = std::make_unique<Renderer>();
    m_input = std::make_unique<Input>();
    m_audio = std::make_unique<Audio>();

    m_renderer->Initialize();
    m_renderer->CreateWindow("Game Engine", 1500, 800);
    m_input->Initialize();
    m_audio->Initialize();

    m_time = std::make_unique<Time>();

    m_particleSystem = std::make_unique<ParticleSystem>();

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
    SDL_Event event;
    while (SDL_PollEvent(&event)) // Check if anything has happened in operating system, "while" bc there could be multiple simultaneous events
    {
        if (event.type == SDL_QUIT) // Quit the window
        {
            quit = true;
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) // Press escape
        {
            quit = true;
        }
    }

    m_time->Tick();
    m_input->Update();
    m_audio->Update();
    m_particleSystem->Update(m_time->GetDeltaTime());
}
