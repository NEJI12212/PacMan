//script for the map generation.
#pragma once
#include <iostream>
#include <XEngine.h>


class PacTileMap
{
public:
    static void StaticInitialize();
    static void StaticTerminate();
    static PacTileMap& Get();
public:
    
    enum class TileTypes
    {
        OPEN,
        WALL,
        BALL,
        POWERORB,
        WHITE
    };
    //X engine defaults
    void Load();
    void Render();
    void Unload();
    void Update(float deltatime);

    //Outer layer rendering
    void RenderOutside();

    // map boundary
    int CheckPlayerCollision(const X::Math::LineSegment& lineSegment) const;
    bool CheckCollision(const X::Math::LineSegment& lineSegment) const;
    X::Math::Vector2 GetMaxBoundaries() const;
    bool HitEnemy(const X::Math::Rect player, const X::Math::Rect enemy) const;
    bool GetTeleportFlag() const { return mTeleport; }

    // Score
    bool GetPowerMode() const{ return mPowerMode; }
    void SetPowerTimer() { mPowerTimer = 15.0f; }
private:
    // Get tile
    int GetIndex(int row, int column) const;

    // Tile variables
    std::unique_ptr<int> mTiles;
    unsigned int mColumns = 0;
    unsigned int mRows = 0;

    std::vector<X::TextureId> mTilesTexture;
    
    // Score
    mutable bool mTeleport = false;
    float mPowerTimer = 0.0f;
    bool mPowerMode = false;

};
