#include "Sprites.h"

class StaticSpriteImpl : VisibleObjectImpl {
public:
    void RenderIt() override {
        // magic.do()
    }

    void UpdatePosition(const Position &position) override {
        top_left_ = position;
    }

    void Translate(const Vector2D &vector2D) override {
        top_left_.Translate(vector2D);
    }

private:
    // magic
};

StaticSprite::StaticSprite() {
    impl_ = std::make_unique<StaticSpriteImpl>();
};

class AnimatedSpriteImpl : VisibleObjectImpl {
public:
    void RenderIt() override {
        // magic.do()
    }

    void UpdatePosition(const Position &position) override {
        top_left_ = position;
    }

    void Translate(const Vector2D &vector2D) override {
        top_left_.Translate(vector2D);
    }

private:
    // magic
};

AnimatedSprite::AnimatedSprite() {
    impl_ = std::make_unique<AnimatedSpriteImpl>();
};