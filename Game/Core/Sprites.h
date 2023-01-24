#pragma once

#include <memory>
#include "VisibleObject.h"

class VisibleObjectImpl {
public:
    VisibleObjectImpl() = default;

    virtual void UpdatePosition(const Position& position) = 0;
    virtual void Translate(const Vector2D& vector2D) = 0;

    virtual void RenderIt() = 0;

protected:
    Position top_left_;
    double width_;
    double height_;
};

class StaticSpriteImpl;

class StaticSprite : public VisibleObject {
public:
    StaticSprite();

    void UpdatePosition(const Position& position) override;
    void Translate(const Vector2D& vector2D) override;

    void RenderIt() override;

private:
    std::unique_ptr<StaticSpriteImpl> impl_;
};

class AnimatedSpriteImpl;

class AnimatedSprite : public VisibleObject {
public:
    AnimatedSprite();

    void UpdatePosition(const Position& position) override;
    void Translate(const Vector2D& vector2D) override;

    void RenderIt() override;

private:
    std::unique_ptr<AnimatedSpriteImpl> impl_;
};