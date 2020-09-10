//Main loop for the game.

#include "EnemyManager.h"
#include "PacTileMap.h"
#include "Player.h"
#include <XEngine.h>

Character* player = new Player();
char* buffer = new char[255];
bool debug = false;
X::SoundId pacSong;
bool start = false;

//----------------------------------------------------------------------------------

void GameInit()
{
    PacTileMap::StaticInitialize();
    PacTileMap::Get().Load();

    EnemyManager::StaticInitialize();
    EnemyManager::Get().Load();

    player->SetPosition({ 48.0f, 244.0f });
    player->Load();

    X::SetBackgroundColor(X::Colors::Black);
    pacSong = X::LoadSound("PacMan_intro_music.wav");
}

//----------------------------------------------------------------------------------

void GameCleanUp()
{
    PacTileMap::Get().Unload();
    PacTileMap::StaticTerminate();

    EnemyManager::Get().Unload();
    EnemyManager::StaticTerminate();

    player->Unload();
    delete player;
    player = nullptr;

    delete[] buffer;
    buffer = nullptr;
}

//----------------------------------------------------------------------------------

bool GameLoop(float deltaTime)
{
    PacTileMap::Get().Render();
    player->Render();
    PacTileMap::Get().RenderOutside();
    EnemyManager::Get().Render();

    if (!start)
    {
        X::PlaySoundOneShot(pacSong);
        start = !start;
    }
    if (X::IsSoundPlaying(pacSong))
        return false;
    PacTileMap::Get().Update(deltaTime);
    player->Update(deltaTime);
    EnemyManager::Get().Update(deltaTime);


    X::Math::Rect bounds = player->GetBoundingBox();
    if (debug)
        X::DrawScreenRect(bounds, X::Colors::Red);
    int points = player->GetScore();
    const char* score = itoa(points, buffer, 10);
    X::DrawScreenText(score, 700, 20, 0, X::Colors::Red);
    if (X::IsKeyPressed(X::Keys::GRAVE))
        debug = !debug;
    for (auto enemy : EnemyManager::Get().GetGhosts())
    {
        X::Math::Rect enemyBounds = enemy->GetBoundingBox();
        if (debug)
            X::DrawScreenRect(enemyBounds, X::Colors::White);
        if (PacTileMap::Get().HitEnemy(bounds, enemyBounds))
        {
            if (!PacTileMap::Get().GetPowerMode())
                return true;
            else
            {
                enemy->SetAlive();
                enemy->SetRevive(10.0f);
            }
        }
    }
    return X::IsKeyPressed(X::Keys::ESCAPE);
    
}

//----------------------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    X::Start();
    GameInit();

    X::Run(GameLoop);

    GameCleanUp();
    X::Stop();
    return 0;
}