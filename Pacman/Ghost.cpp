#include "Ghost.h"

namespace {
    // texture size magic number
    constexpr float textureSize = 16.0f;
}

//----------------------------------------------------------------------------------

void Ghost::Load()
{
    // load ghost sprites
    mPowerSprite.push_back(X::LoadTexture("ghost_ded1.png"));
    mPowerSprite.push_back(X::LoadTexture("ghost_ded1.png"));
    mPowerSprite.push_back(X::LoadTexture("ghost_ded2.png"));
    mPowerSprite.push_back(X::LoadTexture("ghost_ded2.png"));
    if (mColour == GHOST_COLOUR::RED)
    {
        mEnemySprite.push_back(X::LoadTexture("red_ghost1.png"));
        mEnemySprite.push_back(X::LoadTexture("red_ghost1.png"));
        mEnemySprite.push_back(X::LoadTexture("red_ghost2.png"));
        mEnemySprite.push_back(X::LoadTexture("red_ghost2.png"));
        mHeading = { 1, 0 };
    }
    else if (mColour == GHOST_COLOUR::BLUE)
    {
        mEnemySprite.push_back(X::LoadTexture("blue_ghost1.png"));
        mEnemySprite.push_back(X::LoadTexture("blue_ghost1.png"));
        mEnemySprite.push_back(X::LoadTexture("blue_ghost2.png"));
        mEnemySprite.push_back(X::LoadTexture("blue_ghost2.png"));
        mHeading = { 0, 1 };
    }
    else if (mColour == GHOST_COLOUR::PINK)
    {
        mEnemySprite.push_back(X::LoadTexture("pink_ghost1.png"));
        mEnemySprite.push_back(X::LoadTexture("pink_ghost1.png"));
        mEnemySprite.push_back(X::LoadTexture("pink_ghost2.png"));
        mEnemySprite.push_back(X::LoadTexture("pink_ghost2.png"));
        mHeading = { 0, 1 };
    }
    else if (mColour == GHOST_COLOUR::PURPLE)
    {
        mEnemySprite.push_back(X::LoadTexture("purple_ghost1.png"));
        mEnemySprite.push_back(X::LoadTexture("purple_ghost1.png"));
        mEnemySprite.push_back(X::LoadTexture("purple_ghost2.png"));
        mEnemySprite.push_back(X::LoadTexture("purple_ghost2.png"));
        mHeading = { -1, 0 };
    }
    else if (mColour == GHOST_COLOUR::ORANGE)
    {
        mEnemySprite.push_back(X::LoadTexture("orange_ghost1.png"));
        mEnemySprite.push_back(X::LoadTexture("orange_ghost1.png"));
        mEnemySprite.push_back(X::LoadTexture("orange_ghost2.png"));
        mEnemySprite.push_back(X::LoadTexture("orange_ghost2.png"));
        mHeading = { 0, -1 };
    }
    else
    {
        XASSERT(true, "There was no colour initialized to this ghost");
    }
}

//----------------------------------------------------------------------------------

void Ghost::Render()
{
    // render ghost
    if (!mIsAlive)
        return;

    if (mCurrentSprite == mEnemySprite.size())
        mCurrentSprite = 0;
    if(!PacTileMap::Get().GetPowerMode())
        X::DrawSprite(mEnemySprite[mCurrentSprite], mPosition);
    else
        X::DrawSprite(mPowerSprite[mCurrentSprite], mPosition);
    mCurrentSprite++;
}

//----------------------------------------------------------------------------------

void Ghost::Unload()
{
    // clear out ghost
    mEnemySprite.clear();
}

//----------------------------------------------------------------------------------

void Ghost::Update(float deltaTime)
{
    // update ghost enemy
    if (mRevive < 0)
    {
        mRevive = 1000.0f;
        mPosition = { 282.0f,240.0f };
        mIsAlive = true;
    }
    mRevive -= deltaTime;
    if (!mIsAlive)
    {
        mPosition = { 0,0 };
        return;
    }
    const float moveSpeed = PacTileMap::Get().GetPowerMode()? 50.0f : 100.0f;
    mPosition += mHeading * moveSpeed * deltaTime;
}

//----------------------------------------------------------------------------------

X::Math::Rect Ghost::GetBoundingBox() const
{
    // return the bounding box for the ghost.
    // the 6 magic number is used to give extra space to go down coridoors
    return {
        mPosition.x - ((textureSize / 2.0f) - 6),
        mPosition.y - ((textureSize / 2.0f) - 6),
        mPosition.x + ((textureSize / 2.0f) - 6),
        mPosition.y + ((textureSize / 2.0f) - 6),
    };
}