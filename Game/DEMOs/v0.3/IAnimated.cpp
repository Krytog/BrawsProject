#include "IAnimated.h"

void IAnimated::AnimationsInitialization(const CommonAnimationPack& packs,
                                         const InterruptPoints& interrupt_points) {
    if (visible_object_.get()) {
        throw std::runtime_error("Character: visible object has been already initialized!");
    }

    /* Add memory leaks defence with smart ptrs */
    std::unordered_map<std::string_view, std::unique_ptr<VisibleObject>> unique_objects;

    AnimationSequencer::NamedVisibleObjectsList objects_list;
    for (const auto& [name, pack] : packs) {
        VisibleObject* object;
        if (auto static_pack = std::get_if<TempStaticSpriteArgPack>(&pack)) {
            unique_objects[name] = std::make_unique<StaticSprite>(position_.get(), static_pack->width, static_pack->height,
                                                                  static_pack->path, static_pack->render_level);
        } else if (auto animated_pack = std::get_if<TempAnimatedSpriteArgPack>(&pack)) {
            unique_objects[name] = std::make_unique<AnimatedSprite>(position_.get(), animated_pack->width, animated_pack->height, animated_pack->path,
                animated_pack->render_level, animated_pack->ticks_per_frame, animated_pack->columns,
                animated_pack->rows, animated_pack->interrupt_points, animated_pack->cycled);
        } else {
            throw std::runtime_error("Invalid pack structure");
        }
    }

    for (auto& [name, unique]: unique_objects) {
        objects_list.emplace_back(name, unique.release());
    }

    visible_object_ = std::make_unique<AnimationSequencer>(objects_list, interrupt_points, true);
    engine_->RenderSwith(this, visible_object_.get());
    AddAnimationDependences();
}