#include "SpaceGame.h"
#include "Scene.h"
#include "Engine.h"
#include "Player.h"
#include "Enemy.h"
#include "GameData.h"
#include "Particle.h"

bool SpaceGame::Initialize()
{
    m_scene = new Scene(this);

    m_avignon->Load("Avignon.ttf", 40);
    m_avignonSmall->Load("Avignon.ttf", 20);
    m_avignonItalic->Load("Avignon-Italic.ttf", 150);
    m_homeworld->Load("homew.ttf", 100);

    m_titleText->Create(g_engine.GetRenderer(), "SPACE GAME", Color{ 1, 1, 1 });
    m_subtitleText->Create(g_engine.GetRenderer(), "S P A C E G A M E", Color{ 0.0f, 0.2f, 0.2f });
    m_startText->Create(g_engine.GetRenderer(), "PRESS  SPACE  TO  PLAY", Color{ 1, 1, 1 });
    m_livesText->Create(g_engine.GetRenderer(), "Remaining  deployments", Color{ 1, 1, 1 });
    m_healthText->Create(g_engine.GetRenderer(), "Hull integrity", Color{ 1, 1, 1 });

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
        m_textTimer -= dt;
        if (m_textTimer <= -0.5) m_textTimer = 0.5;
        m_scene->RemoveAll();
        if (!m_scene->HasStars()) m_scene->AddStars();
        if (m_engine->GetInput().getKeyDown(SDL_SCANCODE_SPACE))
        {
            m_state = State::START_GAME;
        }
        break;
    case State::START_GAME:
        m_score = 0;
        m_lives = 3;

        m_livesDisplay.clear();
        for (int i = 0; i < m_lives; i++) {
            m_livesDisplay.push_back(Particle{ {345 + (i * 30), 38 }, { 0, 0 }, -42, { 0, 1, 0 }, 2 });
        }

        m_state = State::START_LEVEL;
        break;
    case State::START_LEVEL:
        m_health = 6;

        m_healthDisplay.clear();
        for (int i = 0; i < m_health; i++) {
            m_healthDisplay.push_back(Particle{ {1275 - (i * 30), 38 }, { 0, 0 }, -42, { 0, 1, 1 }, 2 });
        }
        {
            // Curly brackets to limit the scope in here and not in the switch statement
            Model* model = new Model{ GameData::shipPoints, Color{ 0, 0.5, 0.5 } };
            Transform transform{ { g_engine.GetRenderer().GetWidth() >> 1, g_engine.GetRenderer().GetHeight() >> 1 }, 0, 3 }; // >> 1 is bitshift right, exactly the same as /2 but it looks fancier

            Player* player = new Player(0.4f, 0.6f, transform, model);
            player->SetDamping(0); // There's no air in space
            player->SetTag("Player");

            m_scene->AddActor(player);
        }
        m_spawnTime = 2;
        m_spawnTimer = m_spawnTime;
        
        m_state = State::GAME;
        break;
    case State::GAME:
        m_spawnTimer -= dt;
        if (m_spawnTimer <= 0)
        {
            m_spawnTime = 10;
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
    m_scene->Draw(renderer);
    switch (m_state)
    {
    case State::TITLE:
        m_titleText->Draw(g_engine.GetRenderer(), 220, 200);
        m_subtitleText->Draw(g_engine.GetRenderer(), 220, 330);
        if(m_textTimer > 0) m_startText->Draw(g_engine.GetRenderer(), 480, 600);
        // Draw the game title
        break;
    case State::GAME:
    case State::PLAYER_DEAD:
        m_livesText->Draw(g_engine.GetRenderer(), 30, 30);
        m_healthText->Draw(g_engine.GetRenderer(), 1300, 30);

        for (Particle particle : m_livesDisplay) 
        {
            particle.Draw(g_engine.GetRenderer());
        }

        for (Particle particle : m_healthDisplay)
        {
            particle.Draw(g_engine.GetRenderer());
        }
        // Draw score and lives
        break;
    case State::GAME_OVER:
        // Draw the end screen
        break;
    default:
        break;
    }
}

void SpaceGame::OnPlayerDeath()
{
    m_lives--;
    m_livesDisplay[m_lives].SetColor(Color{ 0.3f, 0.3f, 0.3f });
    m_state = (m_lives <= 0) ? State::GAME_OVER : State::PLAYER_DEAD;
    m_stateTimer = 3;
}

void SpaceGame::OnPlayerDamage(int damage)
{
    m_health -= damage;
    for (int i = m_healthDisplay.size() - 1; (i >= m_health && i >= 0); i--)
    {
        m_healthDisplay[i].SetColor(Color{ 0.3f, 0.3f, 0.3f });
    }
}
