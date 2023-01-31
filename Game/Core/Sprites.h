#pragma once

#include <memory>
#include <string_view>
#include <unordered_set>
#include <queue>

#include "VisibleObject.h"
#include "BasicSequncer.h"

class StaticSpriteImpl;

class StaticSprite : public VisibleObject {
public:
    StaticSprite(const Position* pos, const size_t& width, const size_t& height,
                 std::string_view path_to_file, const size_t& render_level);

    void UpdatePosition(const Position& position) override;
    void Translate(const Vector2D& vector2D) override;

    void RenderIt(Canvas* canvas) const override;
    size_t GetRenderLevel() const override;

    bool IsFinished() const override;
    bool IsAbleToInterrupt() const override;
    void Reset() override;

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
                   const size_t& frames_count_height, const std::unordered_set<size_t>& interrupt_points = {}, bool is_cycled = true);

    void UpdatePosition(const Position& position) override;
    void Translate(const Vector2D& vector2D) override;

    void RenderIt(Canvas* canvas) const override;
    size_t GetRenderLevel() const override;

    bool IsFinished() const override;
    bool IsAbleToInterrupt() const override;
    void Reset() override;

    ~AnimatedSprite();

private:
    std::unique_ptr<AnimatedSpriteImpl> impl_;
};

class AnimationSequencer : public VisibleObject, public BasicSequencer<VisibleObject> {
public:
    enum SwitchOption {
        FORCE, /* Change animation immediately */
        MIXED, /* Change after first breakpoint, or animation is finished */
        SOFT   /* Change after current animation is finished( !Cyclic objects are never finished! ) */
    };

public:
    AnimationSequencer(const std::vector<std::pair<std::string_view, VisibleObject*>> &params_list,
                       const std::unordered_set<std::string_view> &interrupt_points = {},
                       bool is_cycled = true);

    void SwitchAnimationTo(std::string_view tag, SwitchOption option);

    void RenderIt(Canvas *canvas) const override;
    void UpdatePosition(const Position &position) override;
    void Translate(const Vector2D &vector2D) override;

    bool IsFinished() const override;
    bool IsAbleToInterrupt() const override;
    size_t GetRenderLevel() const override;

    void Reset() override;

private:
    struct SwitchInfo {
        std::string_view tag;
        SwitchOption option;
    };

    std::unordered_set<std::string_view> interrupt_points_;
    mutable std::queue<SwitchInfo> switch_queue_;
};