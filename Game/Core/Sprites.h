#pragma once

#include "VisibleObject.h"

class VisibleObjectImpl {
public:
    VisibleObjectImpl() = default;

    virtual void RenderIt() = 0;
private:
    Position top_left_;
    double width_;
    double height_;
};

class StaticSpriteImpl;

class StaticSprite: public VisibleObject {
public:
    StaticSprite();

    void RenderIt() override;
private:
    std::unique_ptr<StaticSpriteImpl> impl_;
};

class AnimatedSpriteImpl;

class AnimatedSprite: public VisibleObject {
public:
    AnimatedSprite();

    void RenderIt() override;
private:
    std::unique_ptr<AnimatedSpriteImpl> impl_;
};