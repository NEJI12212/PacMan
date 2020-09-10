#pragma once

#include <XEngine.h>

class Character
{
public:
    Character(){}
    virtual ~Character() {};

    // X engine defaults
    virtual void Load() = 0;
    virtual void Render() = 0;
    virtual void Unload() = 0;
    virtual void Update(float deltatime) = 0;

    // position functions
    virtual const X::Math::Vector2& GetPosition() const = 0;
    virtual void SetPosition(const X::Math::Vector2& position) = 0;
    virtual X::Math::Rect GetBoundingBox() const = 0;

    //score
    virtual int GetScore() const = 0;
protected:
    // position variables
    X::Math::Vector2 mPosition;
    X::Math::Vector2 mHeading;
};
