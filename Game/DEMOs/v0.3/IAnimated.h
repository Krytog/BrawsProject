#pragma once

#include "../../Core/CustomBehaviour.h"
#include <variant>
#include <unordered_map>
#include <string_view>

struct TempStaticSpriteArgPack {
    double width;
    double height;
    std::string_view path;
    LEVELS render_level;
};

struct TempAnimatedSpriteArgPack {
    double width;
    double height;
    std::string_view path;
    LEVELS render_level;
    uint8_t ticks_per_frame;
    uint64_t columns;
    uint64_t rows;
    std::unordered_set<size_t> interrupt_points;
    bool cycled;
};

class IAnimated : public virtual GameObject, public virtual CustomBehaviour {
public:
    IAnimated() = default;

    using VisObjArgPack = std::variant<TempStaticSpriteArgPack, TempAnimatedSpriteArgPack>;

    using CommonAnimationPack = std::unordered_map<std::string_view, VisObjArgPack>;
    using InterruptPoints = std::unordered_set<std::string_view>;

    ~IAnimated() = default;

protected:
    void AnimationsInitialization(const CommonAnimationPack& packs, const InterruptPoints& interrupt_points);
    virtual void AddAnimationDependences() = 0;
};