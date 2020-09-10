#pragma once

#include "Character.h"
#include <XEngine.h>
#include "PacTileMap.h"

class Player : public Character
{
public:
    //X engine defaults
    virtual void Load();
    virtual void Render();
    virtual void Unload();
    virtual void Update(float deltatime);

    // position functions
    virtual const X::Math::Vector2& GetPosition() const { return mPosition; }
    virtual void SetPosition(const X::Math::Vector2& position) { mPosition = position; }
    X::Math::Rect GetBoundingBox() const;

    //points
    int GetScore() const { return mPoints; }
private:
    // check if we collided anything on the map
    void PlayerCollision(X::Math::Vector2& offset);
    bool TileCollision(X::Math::LineSegment& edge);

    // sprite variables
    std::vector<X::TextureId> mCharacterSprite;
    int mCurrentSprite = 0;
    mutable bool mMoving = false;

    // points
    int mPoints = 0;

};
