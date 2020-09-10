#pragma once

#include "Ghost.h"
#include "PacTileMap.h"
#include <XEngine.h>


class EnemyManager
{
//Singleton init
public:
    static void StaticInitialize();
    static void StaticTerminate();
    static EnemyManager& Get();

public:
    //X engine defaults
    void Load();
    void Unload();
    void Update(float deltaTime);
    void Render();

    // get the ghost enemies
    std::vector<Ghost*> GetGhosts() const { return mEnemies; }
private:
    bool randomBool();
    void CreateGhost(X::Math::Vector2 pos, Ghost::GHOST_COLOUR colour);
    void MovementLogic(float deltaTime);
    std::vector<Ghost*> mEnemies;
};