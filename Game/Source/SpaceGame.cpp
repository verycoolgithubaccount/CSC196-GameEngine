#include "SpaceGame.h"
#include "Scene.h"
#include "Engine.h"
#include "Player.h"
#include "Enemy.h"
#include "Ally.h"
#include "GameData.h"
#include "Particle.h"
#include "MathUtils.h"
#include <iostream>

bool SpaceGame::Initialize()
{
    m_scene = new Scene(this);

    m_avignon->Load("Avignon.ttf", 40);
    m_avignonSmall->Load("Avignon.ttf", 20);
    m_avignonItalic->Load("Avignon-Italic.ttf", 150);
    m_homeworld->Load("homew.ttf", 100);
    m_begok->Load("Begokv15.ttf", 20);

    m_titleText->Create(g_engine.GetRenderer(), "SPACE GAME", Color{ 1, 1, 1 });
    m_subtitleText->Create(g_engine.GetRenderer(), "S P A C E G A M E", Color{ 0.0f, 0.2f, 0.2f });
    m_startText->Create(g_engine.GetRenderer(), "PRESS  SPACE  TO  PLAY", Color{ 1, 1, 1 });
    m_livesText->Create(g_engine.GetRenderer(), "Remaining  deployments", Color{ 1, 1, 1 });
    m_healthText->Create(g_engine.GetRenderer(), "Ship integrity", Color{ 1, 1, 1 });
    m_scoreText->Create(g_engine.GetRenderer(), "score: 0", Color{ 1, 1, 1 });
    m_laserEnergyText->Create(g_engine.GetRenderer(), "Laser energy", Color{ 1, 1, 1 });

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
        m_laserEnergy = 0;

        m_healthDisplay.clear();
        for (int i = 0; i < m_health; i++) {
            m_healthDisplay.push_back(Particle{ {1275 - (i * 30), 38 }, { 0, 0 }, -42, { 0, 1, 1 }, 2 });
        }
        {
            // Curly brackets to limit the scope in here and not in the switch statement
            Model* model = new Model{ GameData::shipPoints, Color{ 0, 0.5, 0.5 } };
            Transform transform{ { g_engine.GetRenderer().GetWidth() >> 1, g_engine.GetRenderer().GetHeight() >> 1 }, 0, 3 }; // >> 1 is bitshift right, exactly the same as /2 but it looks fancier

            auto player = std::make_unique<Player>(0.6f, 0.6f, transform, model);
            player->SetDamping(0); // There's no air in space
            player->SetTag("Player");

            m_scene->AddActor(std::move(player));
        }
        m_spawnTime = 2;
        m_spawnTimer = m_spawnTime;
        
        m_state = State::GAME;
        break;
    case State::GAME:
        AddLaserEnergy(dt);
        m_laserEnergyDisplay.clear();
        for (int i = 0; i < MAX_LASER_ENERGY; i++) {
            m_laserEnergyDisplay.push_back(Particle{ {50, g_engine.GetRenderer().GetHeight() - 80 - (i * 10)}, {0, 0}, -42, {0, 1, 1}, 3});
        }
        for (int i = m_laserEnergyDisplay.size() - 1; (i >= m_laserEnergy && i >= 0); i--)
        {
            m_laserEnergyDisplay[i].SetColor(Color{ 0.25f, 0.25f, 0.25f });
        }

        m_spawnTimer -= dt;
        if (m_health < 3) {
            m_healthWarningTimer -= dt;
            if (m_healthWarningTimer <= 0)
            {
                m_healthWarningTimer = 3;
                g_engine.GetAudio().PlaySound("low_health.wav");
            }
        }
        if (m_spawnTimer <= 0)
        {
            m_spawnTime = 10;
            m_spawnTimer = m_spawnTime;
            if (randomf() > 0)
            {
                Model* model = new Model{ GameData::shipPoints, Color{ 0.5f, 0.0f, 0.0f } };
                auto new_enemy = std::make_unique<Enemy>(0.4f, 0.6f, Transform{ { random(g_engine.GetRenderer().GetWidth()), random(g_engine.GetRenderer().GetHeight()) }, 0, 3 }, model);
                new_enemy->SetTag("Enemy");
                m_scene->AddActor(std::move(new_enemy));
            }
            if (randomf() > 0.5)
            {
                Model* model = new Model{ GameData::shipPoints, Color{ 0.0f, 0.5f, 0.0f } };
                auto new_ally = std::make_unique<Ally>(0.4f, 0.6f, Transform{ { random(g_engine.GetRenderer().GetWidth()), random(g_engine.GetRenderer().GetHeight()) }, 0, 3 }, model);
                new_ally->SetTag("Ally");
                m_scene->AddActor(std::move(new_ally));
            }
        }
        break;
    case State::PLAYER_DEAD:
        {
            Player* player = m_scene->GetActor<Player>();
            if (player) {
                player->SetControlsDisabled(true);
                if (randomf() > 0.983f) {
                    Vector2 direction = Vector2{ randomf(-1, 1), randomf(-1, 1) }.Normalized();
                    for (int i = 0; i < 50; i++)
                    {
                        Particle::Data data
                        {
                            player->GetTransform().translation + ((direction * 4 * player->GetTransform().scale) + randomf(0, 3)),
                            player->GetVelocity() + (direction * random(0, 200)) + randomf(0, 10),
                            randomf(0, 1),
                            Color{ 1, randomf(), randomf(0.0f, 0.3f) },
                            randomf(2.8f * player->GetTransform().scale, 3.8f * player->GetTransform().scale)
                        };
                        g_engine.GetParticleSystem().AddParticle(data);
                    }
                }
            }


            m_stateTimer -= dt;
            if (m_stateTimer <= 3 && player) {
                player->OnDeath();
            }
            if (m_stateTimer <= 0) {
                m_state = State::START_LEVEL;
            }
        }
        break;
    case State::GAME_OVER:
        {
            Player* player = m_scene->GetActor<Player>();
            if (player) {
                player->SetControlsDisabled(true);
                if (randomf() > 0.983f) {
                    Vector2 direction = Vector2{ randomf(-1, 1), randomf(-1, 1) }.Normalized();
                    for (int i = 0; i < 50; i++)
                    {
                        Particle::Data data
                        {
                            player->GetTransform().translation + ((direction * 4 * player->GetTransform().scale) + randomf(0, 3)),
                            player->GetVelocity() + (direction * random(0, 200)) + randomf(0, 10),
                            randomf(0, 1),
                            Color{ 1, randomf(), randomf(0.0f, 0.3f) },
                            randomf(2.8f * player->GetTransform().scale, 3.8f * player->GetTransform().scale)
                        };
                        g_engine.GetParticleSystem().AddParticle(data);
                    }
                }
            }


            m_stateTimer -= dt;
            if (m_stateTimer <= 3 && player) {
                player->OnDeath();
            }
            if (m_stateTimer <= 0) {
                m_state = State::TITLE;
            }
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
    case State::GAME_OVER:
        m_scoreText->Create(g_engine.GetRenderer(), "score: " + std::to_string(m_score), Color{ 1, 1, 1 });
        m_scoreText->Draw(g_engine.GetRenderer(), 680, 30);
        m_livesText->Draw(g_engine.GetRenderer(), 30, 30);
        m_healthText->Draw(g_engine.GetRenderer(), 1300, 30);
        m_laserEnergyText->Draw(g_engine.GetRenderer(), 30, g_engine.GetRenderer().GetHeight() - 45);

        for (Particle particle : m_livesDisplay) 
        {
            particle.Draw(g_engine.GetRenderer());
        }

        for (Particle particle : m_healthDisplay)
        {
            particle.Draw(g_engine.GetRenderer());
        }

        for (Particle particle : m_laserEnergyDisplay)
        {
            particle.Draw(g_engine.GetRenderer());
        }
        break;
    default:
        break;
    }
}

void SpaceGame::OnPlayerDeath()
{
    g_engine.GetAudio().PlaySound("death.wav");
    m_lives--;
    for (int i = m_livesDisplay.size() - 1; (i >= m_lives && i >= 0); i--)
    {
        m_livesDisplay[i].SetColor(Color{ 0.3f, 0.3f, 0.3f });
    }
    m_state = (m_lives <= 0) ? State::GAME_OVER : State::PLAYER_DEAD;
    m_stateTimer = 6;
}

void SpaceGame::OnPlayerDamage(int damage)
{
    m_health -= damage;
    for (int i = m_healthDisplay.size() - 1; (i >= m_health && i >= 0); i--)
    {
        m_healthDisplay[i].SetColor(Color{ 0.3f, 0.3f, 0.3f });
    }
}

void SpaceGame::AddLaserEnergy(float modifier)
{
    m_laserEnergy = Math::Clamp(m_laserEnergy + modifier, 0.0f, MAX_LASER_ENERGY);
}
