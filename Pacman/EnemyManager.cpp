#include "EnemyManager.h"

namespace
{
    // instance for the enemy manager
    EnemyManager* sInstance = nullptr;
}

//----------------------------------------------------------------------------------

void EnemyManager::StaticInitialize()
{
    // create the enemy manager
    XASSERT(sInstance == nullptr, "EnemyManager already initialized!");
    sInstance = new EnemyManager();
}

//----------------------------------------------------------------------------------

void EnemyManager::StaticTerminate()
{
    // delete the manager
    delete sInstance;
    sInstance = nullptr;
}

//----------------------------------------------------------------------------------

EnemyManager& EnemyManager::Get()
{
    // get the manager instance
    XASSERT(sInstance != nullptr, "No EnemyManager created!");
    return *sInstance;
}

//----------------------------------------------------------------------------------

void EnemyManager::Load()
{
    // create ghosts
    CreateGhost({ 106.0f, 106.0f }, Ghost::GHOST_COLOUR::RED);
    CreateGhost({ 38.0f, 38.0f }, Ghost::GHOST_COLOUR::BLUE);
    CreateGhost({ 38.0f, 60.0f }, Ghost::GHOST_COLOUR::PINK);
    CreateGhost({ 60.0f, 342.0f }, Ghost::GHOST_COLOUR::PURPLE);
    CreateGhost({ 138.0f, 342.0f }, Ghost::GHOST_COLOUR::ORANGE);
    CreateGhost({ 138.0f, 242.0f }, Ghost::GHOST_COLOUR::ORANGE);
    CreateGhost({ 208.0f, 142.0f }, Ghost::GHOST_COLOUR::ORANGE);
}

//----------------------------------------------------------------------------------

void EnemyManager::Unload()
{
    // unload all enemies
    for (auto enemy : mEnemies)
    {
        enemy->Unload();
        delete enemy;
        enemy = nullptr;
    }
}

//----------------------------------------------------------------------------------

void EnemyManager::Update(float deltaTime)
{
    //update enemies
    MovementLogic(deltaTime);
}

//----------------------------------------------------------------------------------

void EnemyManager::Render()
{
    // render enemies
    for (auto enemy : mEnemies)
        enemy->Render();
}

//----------------------------------------------------------------------------------

void EnemyManager::CreateGhost(X::Math::Vector2 pos, Ghost::GHOST_COLOUR colour)
{
    // instantiate a ghost enemy
    Ghost* enemy = new Ghost(colour);
    enemy->Load();
    enemy->SetPosition(pos);
    mEnemies.push_back(enemy);
}

//----------------------------------------------------------------------------------

void EnemyManager::MovementLogic(float deltaTime)
{
    // Movement for the ghosts
    for (auto enemy : mEnemies)
    {
        X::Math::Rect bounds = enemy->GetBoundingBox();
        X::Math::Vector2 heading = enemy->GetHeading();
        X::Math::Vector2 newHeading = { 0, 0 };
        if (heading.x > 0.0f)
        {
            X::Math::LineSegment rightEdge{
                bounds.max.x + heading.x,
                bounds.min.y,
                bounds.max.x + heading.x,
                bounds.max.y,
            };
            if (PacTileMap::Get().CheckCollision(rightEdge))
            {
                if (enemy->GetColour() == Ghost::GHOST_COLOUR::PINK ||
                    enemy->GetColour() == Ghost::GHOST_COLOUR::PURPLE)
                {
                    newHeading.y = enemy->GetColour() == Ghost::GHOST_COLOUR::PINK ? -1.0f : 1.0f;
                    enemy->SetHeading(newHeading);
                    X::Math::Vector2 pos = enemy->GetPosition();
                    pos.x -= 4.0f;
                    enemy->SetPosition(pos);
                }
                else if (enemy->GetColour() == Ghost::GHOST_COLOUR::ORANGE)
                {
                    if (randomBool())
                        newHeading.x = -1.0f;
                    else
                        newHeading.y = randomBool()? -1.0f : 1.0f;
                    enemy->SetHeading(newHeading);
                    X::Math::Vector2 pos = enemy->GetPosition();
                    pos.x -= 4.0f;
                    enemy->SetPosition(pos);
                }
                else
                {
                    heading.x = -1.0f;
                    enemy->SetHeading(heading);
                }
            }
        }
        //check right side
        if (heading.x < 0.0f)
        {
            X::Math::LineSegment leftEdge{
                bounds.min.x + heading.x,
                bounds.min.y,
                bounds.min.x + heading.x,
                bounds.max.y,
            };
            if (PacTileMap::Get().CheckCollision(leftEdge))
            {
                if (enemy->GetColour() == Ghost::GHOST_COLOUR::PINK ||
                    enemy->GetColour() == Ghost::GHOST_COLOUR::PURPLE)
                {
                    newHeading.y = enemy->GetColour() == Ghost::GHOST_COLOUR::PINK ? 1.0f : -1.0f;
                    enemy->SetHeading(newHeading);
                    X::Math::Vector2 pos = enemy->GetPosition();
                    pos.x += 4.0f;
                    enemy->SetPosition(pos);
                }
                else if (enemy->GetColour() == Ghost::GHOST_COLOUR::ORANGE)
                {
                    if (randomBool())
                        newHeading.x = 1.0f;
                    else
                        newHeading.y = randomBool() ? -1.0f : 1.0f;
                    enemy->SetHeading(newHeading);
                    X::Math::Vector2 pos = enemy->GetPosition();
                    pos.x += 4.0f;
                    enemy->SetPosition(pos);
                }
                else
                {
                    heading.x = 1.0f;
                    enemy->SetHeading(heading);
                }
            }
        }
        //check top side
        if (heading.y > 0.0f)
        {
            X::Math::LineSegment bottomEdge{
                bounds.min.x,
                bounds.max.y + heading.y,
                bounds.max.x,
                bounds.max.y + heading.y,
            };
            if (PacTileMap::Get().CheckCollision(bottomEdge))
            {
                if (enemy->GetColour() == Ghost::GHOST_COLOUR::PINK ||
                    enemy->GetColour() == Ghost::GHOST_COLOUR::PURPLE)
                {
                    newHeading.x = enemy->GetColour() == Ghost::GHOST_COLOUR::PINK ? 1.0f : -1.0f;
                    enemy->SetHeading(newHeading);
                    X::Math::Vector2 pos = enemy->GetPosition();
                    pos.y -= 4.0f;
                    enemy->SetPosition(pos);
                }
                else if (enemy->GetColour() == Ghost::GHOST_COLOUR::ORANGE)
                {
                    if (randomBool())
                        newHeading.x = randomBool() ? -1.0f : 1.0f;
                    else
                        newHeading.y = -1.0f;
                    enemy->SetHeading(newHeading);
                    X::Math::Vector2 pos = enemy->GetPosition();
                    pos.y -= 4.0f;
                    enemy->SetPosition(pos);
                }
                else
                {
                    heading.y = -1.0f;
                    enemy->SetHeading(heading);
                }
            }
        }
        // check bottom side
        if (heading.y < 0.0f)
        {
            X::Math::LineSegment topEdge{
                bounds.min.x,
                bounds.min.y + heading.y,
                bounds.max.x,
                bounds.min.y + heading.y,
            };
            if (PacTileMap::Get().CheckCollision(topEdge))
            {
                if (enemy->GetColour() == Ghost::GHOST_COLOUR::PINK ||
                    enemy->GetColour() == Ghost::GHOST_COLOUR::PURPLE)
                {
                    newHeading.x = enemy->GetColour() == Ghost::GHOST_COLOUR::PINK ? -1.0f : 1.0f;
                    enemy->SetHeading(newHeading);
                    X::Math::Vector2 pos = enemy->GetPosition();
                    pos.y += 4.0f;
                    enemy->SetPosition(pos);
                }
                else if (enemy->GetColour() == Ghost::GHOST_COLOUR::ORANGE)
                {
                    if (randomBool())
                        newHeading.x = randomBool() ? -1.0f : 1.0f;
                    else
                        newHeading.y = 1.0f;
                    enemy->SetHeading(newHeading);
                    X::Math::Vector2 pos = enemy->GetPosition();
                    pos.y += 4.0f;
                    enemy->SetPosition(pos);
                }
                else
                {
                    heading.y = 1.0f;
                    enemy->SetHeading(heading);
                }
            }
        }
        enemy->Update(deltaTime);
    }
}

//----------------------------------------------------------------------------------

bool EnemyManager::randomBool()
{
    // get a random bool for AI
    static auto gen = std::bind(std::uniform_int_distribution<>(0, 1), std::default_random_engine());
    return gen();
}