#include "Sprites.h"
#include <memory>

class StaticSpriteImpl: VisibleObjectImpl {
public:
    void RenderIt() override {
        // magic.do()
    }
private:
    // magic
};

StaticSprite::StaticSprite() {
    impl_ = std::make_unique<StaticSpriteImpl>();
};

class AnimatedSpriteImpl: VisibleObjectImpl {
public:
    void RenderIt() override {
        // magic.do()
    }
private:
    // magic
};

AnimatedSprite::AnimatedSprite() {
    impl_ = std::make_unique<AnimatedSpriteImpl>();
};