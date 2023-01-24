#pragma once

#include <memory>
#include <string_view>
#include "VisibleObject.h"

class StaticSpriteImpl;

class StaticSprite : public VisibleObject {
public:
    StaticSprite(const Position* pos, const size_t& width, const size_t& height,
                 std::string_view path_to_file, const size_t& render_level);

    void UpdatePosition(const Position& position) override;
    void Translate(const Vector2D& vector2D) override;

    void RenderIt(Canvas* canvas) const override;
    size_t GetRenderLevel() const override;

    ~StaticSprite();

private:
    std::unique_ptr<StaticSpriteImpl> impl_;
};

class AnimatedSpriteImpl;

class AnimatedSprite : public VisibleObject {
public:
    AnimatedSprite(const Position* pos, const size_t& width, const size_t& height,
                   std::string_view path_to_file, const size_t& render_level,
                   const size_t& frame_rate, const size_t& frames_count_width,
                   const size_t& frames_count_height, bool is_cycled = true);

    void UpdatePosition(const Position& position) override;
    void Translate(const Vector2D& vector2D) override;

    void RenderIt(Canvas* canvas) const override;
    size_t GetRenderLevel() const override;

    bool AnimationIsFinished() const;

    ~AnimatedSprite();

private:
    std::unique_ptr<AnimatedSpriteImpl> impl_;
};