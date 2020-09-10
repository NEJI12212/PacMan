#pragma once

#include "Character.h"
#include "PacTileMap.h"

class Ghost : public Character
{
public:

    enum class GHOST_COLOUR {
        RED,
        PINK,
        BLUE,
        PURPLE,
        ORANGE
    };

    // assign a colour to the ghost when it is made
    Ghost(GHOST_COLOUR colour) { mColour = colour;}
    //X engine defaults
    virtual void Load();
    virtual void Render();
    virtual void Unload();
    virtual void Update(float deltatime);

    // setters
    virtual void SetPosition(const X::Math::Vector2& position) { mPosition = position; }
    void SetHeading(const X::Math::Vector2 heading) { mHeading = heading; }
    void SetAlive() { mIsAlive = !mIsAlive; }
    void SetRevive(float time) { mRevive = time; }
    //getters
    virtual const X::Math::Vector2& GetPosition() const { return mPosition; }
    X::Math::Rect GetBoundingBox() const;
    X::Math::Vector2 GetHeading() const {return mHeading;}
    GHOST_COLOUR GetColour() const { return mColour; }
    int GetScore() const { return 0; }
private:
    GHOST_COLOUR mColour;
    // sprite variables
    std::vector<X::TextureId> mEnemySprite;
    std::vector<X::TextureId> mPowerSprite;
    int mCurrentSprite = 0;
    bool mMoving = false;

    // alive
    bool mIsAlive = true;
    float mRevive = 1000.0f;
};
