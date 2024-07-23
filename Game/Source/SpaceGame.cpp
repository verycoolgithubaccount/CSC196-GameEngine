#include "SpaceGame.h"
#include "Scene.h"
#include "Engine.h"
#include "Player.h"
#include "Enemy.h"
#include "GameData.h"

bool SpaceGame::Initialize()
{
    m_scene = new Scene(this);
    return true;
}

void SpaceGame::Shutdown()
{
}

void SpaceGame::Update(float dt)
{
    switch (m_state)
    {
    case State::TITLE:
        m_scene->RemoveAll();
        if (m_engine->GetInput().getKeyDown(SDL_SCANCODE_SPACE))
        {
            m_state = State::START_GAME;
        }
        break;
    case State::START_GAME:
        m_score = 0;
        m_lives = 3;
        m_scene->AddStars();
        m_state = State::START_LEVEL;
        break;
    case State::START_LEVEL:
        {
            // Curly brackets to limit the scope in here and not in the switch statement
            Model* model = new Model{ GameData::shipPoints, Color{ 0, 0.5, 0.5 } };
            Transform transform{ { g_engine.GetRenderer().GetWidth() >> 1, g_engine.GetRenderer().GetHeight() >> 1 }, 0, 3 }; // >> 1 is bitshift right, exactly the same as /2 but it looks fancier

            Player* player = new Player(0.4f, 0.6f, transform, model);
            player->SetDamping(0); // There's no air in space
            player->SetTag("Player");

            m_scene->AddActor(player);
        }
        m_spawnTime = 3;
        m_spawnTimer = m_spawnTime;
        
        m_state = State::GAME;
        break;
    case State::GAME:
        m_spawnTimer -= dt;
        if (m_spawnTimer <= 0)
        {
            m_spawnTimer = m_spawnTime;
            Model* model = new Model{ GameData::shipPoints, Color{ 0.5f, 0.0f, 0.0f } };
            Enemy* new_enemy = new Enemy(0.4f, 0.6f, Transform{ { random(g_engine.GetRenderer().GetWidth()), random(g_engine.GetRenderer().GetHeight()) }, 0, 3 }, model);
            new_enemy->SetTag("Enemy");
            m_scene->AddActor(new_enemy);
        }
        break;
    case State::PLAYER_DEAD:
        m_stateTimer -= dt;
        if (m_stateTimer <= 0) {
            m_state = State::START_LEVEL;
        }
        break;
    case State::GAME_OVER:
        m_stateTimer -= dt;
        if (m_stateTimer <= 0) {
            m_state = State::TITLE;
        }
        break;
    default:
        break;
    }

    m_scene->Update(dt);
}

void SpaceGame::Draw(Renderer& renderer)
{
    switch (m_state)
    {
    case State::TITLE:
        // Draw the game title
        break;
    case State::GAME:
    case State::PLAYER_DEAD:
        // Draw score and lives
        break;
    case State::GAME_OVER:
        // Draw the end screen
        break;
    default:
        break;
    }
    m_scene->Draw(renderer);
}

void SpaceGame::OnPlayerDeath()
{
    m_lives--;
    m_state = (m_lives <= 0) ? State::GAME_OVER : State::PLAYER_DEAD;
    m_stateTimer = 3;
}
