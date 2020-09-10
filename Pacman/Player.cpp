#include "Player.h"

namespace {
    // texture size magic number
    constexpr float textureSize = 16.0f;
}

//----------------------------------------------------------------------------------

void Player::Load()
{
    // load the pacman sprite
    mCharacterSprite.push_back(X::LoadTexture("Pacman1_2.png"));
    mCharacterSprite.push_back(X::LoadTexture("Pacman1_2.png"));
    mCharacterSprite.push_back(X::LoadTexture("Pacman2_2.png"));
    mCharacterSprite.push_back(X::LoadTexture("Pacman2_2.png"));
    mCharacterSprite.push_back(X::LoadTexture("Pacman3_2.png"));
    mCharacterSprite.push_back(X::LoadTexture("Pacman3_2.png"));
    mCharacterSprite.push_back(X::LoadTexture("Pacman2_2.png"));
    mCharacterSprite.push_back(X::LoadTexture("Pacman2_2.png"));

}

//----------------------------------------------------------------------------------

void Player::Render()
{
    // render the player sprite and play the animation
    if (mCurrentSprite == mCharacterSprite.size())
        mCurrentSprite = 0;

    float angle = atan2(mHeading.y, mHeading.x);
    X::DrawSprite(mCharacterSprite[mCurrentSprite], mPosition, angle);
    if(mMoving)
        mCurrentSprite++;
}

//----------------------------------------------------------------------------------

void Player::Unload()
{
    // clear out the loaded images
    mCharacterSprite.clear();
}

//----------------------------------------------------------------------------------

void Player::Update(float deltaTime)
{
    //speed and teleport check variables
    bool rightTeleport = false;
    bool leftTeleport = false;
    const float moveSpeed = 200.0f;
    //character movement
    X::Math::Vector2 offset = { 0, 0 };
    if (X::IsKeyDown(X::Keys::D) || X::IsKeyDown(X::Keys::RIGHT))
    {
        mMoving = true;
        mHeading = { 0, 0 };
        offset.x += moveSpeed * deltaTime;
        if (PacTileMap::Get().GetTeleportFlag())
            rightTeleport = true;
    }
    else if (X::IsKeyDown(X::Keys::A) || X::IsKeyDown(X::Keys::LEFT))
    {
        mMoving = true;
        mHeading = { -1, 0 };
        offset.x -= moveSpeed * deltaTime;
        if (PacTileMap::Get().GetTeleportFlag())
            leftTeleport = true;
    }
    else if (X::IsKeyDown(X::Keys::S) || X::IsKeyDown(X::Keys::DOWN))
    {
        mMoving = true;
        mHeading = { 0, 1 };
        offset.y += moveSpeed * deltaTime;
    }
    else if (X::IsKeyDown(X::Keys::W) || X::IsKeyDown(X::Keys::UP))
    {
        mMoving = true;
        mHeading = { 0, -1 };
        offset.y -= moveSpeed * deltaTime;
    }
    else
        mMoving = false;

    // see if the player hits something
    PlayerCollision(offset);

    // move the character
    mPosition += offset;
    if (rightTeleport)
        mPosition.x = textureSize;
    if (leftTeleport)
    {
        float x_bound = PacTileMap::Get().GetMaxBoundaries().x;
        mPosition.x = x_bound - textureSize;

    }
}

//----------------------------------------------------------------------------------

void Player::PlayerCollision(X::Math::Vector2& offset)
{
    auto currentBox = GetBoundingBox();
    // check left side
    if (offset.x > 0.0f)
    {
        X::Math::LineSegment rightEdge{
            currentBox.max.x + offset.x,
            currentBox.min.y,
            currentBox.max.x + offset.x,
            currentBox.max.y,
        };
        if (TileCollision(rightEdge))
            offset.x = 0.0f;
    }
    //check right side
    if (offset.x < 0.0f)
    {
        X::Math::LineSegment leftEdge{
            currentBox.min.x + offset.x,
            currentBox.min.y,
            currentBox.min.x + offset.x,
            currentBox.max.y,
        };
        if (TileCollision(leftEdge))
            offset.x = 0.0f;
    }
    //check top side
    if (offset.y > 0.0f)
    {
        X::Math::LineSegment bottomEdge{
            currentBox.min.x,
            currentBox.max.y + offset.y,
            currentBox.max.x,
            currentBox.max.y + offset.y,
        };
        if (TileCollision(bottomEdge))
            offset.y = 0.0f;
    }
    // check bottom side
    if (offset.y < 0.0f)
    {
        X::Math::LineSegment topEdge{
            currentBox.min.x,
            currentBox.min.y + offset.y,
            currentBox.max.x,
            currentBox.min.y + offset.y,
        };
        if (TileCollision(topEdge))
            offset.y = 0.0f;
    }
}

//----------------------------------------------------------------------------------

bool Player::TileCollision(X::Math::LineSegment& edge)
{
    //return true if we hit a wall to stop momentum
    int collision = PacTileMap::Get().CheckPlayerCollision(edge);
    if (collision == static_cast<int>(PacTileMap::TileTypes::WALL) ||
        collision == static_cast<int>(PacTileMap::TileTypes::WHITE))
        return true;
    else if (collision == static_cast<int>(PacTileMap::TileTypes::BALL))
        mPoints++;
    else if (collision == static_cast<int>(PacTileMap::TileTypes::POWERORB))
    {
        mPoints++;
        PacTileMap::Get().SetPowerTimer();
    }
    return false;
}


//----------------------------------------------------------------------------------

X::Math::Rect Player::GetBoundingBox() const
{
    //Remove half of texture size to get the bounding box on all sides
    //then -4 for extra room to go into coridors
    return{
        mPosition.x - ((textureSize / 2.0f) - 4),
        mPosition.y - ((textureSize / 2.0f) - 4),
        mPosition.x + ((textureSize / 2.0f) - 4),
        mPosition.y + ((textureSize / 2.0f) - 4),
    };
}
