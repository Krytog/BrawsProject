#pragma once

#include <memory>
#include <string_view>
#include "VisibleObject.h"

class StaticSpriteImpl;

class StaticSprite: public VisibleObject {
public:
    StaticSprite(const Position* pos, const double& width, const double& height, std::string_view path_to_file);

    void UpdatePosition(const Position &position) override;
    void Translate(const Vector2D &vector2D) override;

    void RenderIt(Canvas *canvas) const override;
    ~StaticSprite();
private:
    std::unique_ptr<StaticSpriteImpl> impl_;
};

class AnimatedSpriteImpl;

class AnimatedSprite: public VisibleObject {
public:
    AnimatedSprite(const Position* pos, const double& width, const double& height, std::string_view path_to_file, size_t frame_rate);

    void UpdatePosition(const Position &position) override;
    void Translate(const Vector2D &vector2D) override;

    void RenderIt(Canvas *canvas) const override;
    ~AnimatedSprite();
private:
    std::unique_ptr<AnimatedSpriteImpl> impl_;
};