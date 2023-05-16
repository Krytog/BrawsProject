#pragma once

#include "../../Position.h"
#include "../Painters/DummyPainter.h"
#include <functional>

class IVisibleObject {
public:
    enum RenderLayers {
        BackGround = 0,
        Objects = 1,
        Projectiles = 2,
        Projectiles_extra = 3,
        Effects = 4,
        Effects_extra = 5,
        UI = 6,
        UI_extra = 7};
public:
    IVisibleObject() = default;
    IVisibleObject(const Position& position, const double& angle = 0, const RenderLayers& layer = RenderLayers::Objects);

    virtual void UpdatePosition(const Position &position);
    virtual void Translate(const Vector2D &vector2d);

    void UpdateRotation(const double& angle);
    void UpdateRotation(const Vector2D& rotator);
    void Rotate(const double& angle);

    virtual void RenderIt(Painter* painter) = 0;
    RenderLayers GetLayer() const;
    virtual bool IsDisplayed() const = 0;

    Position GetPosition() const { 
        return pos_;
    }

    double GetRotationAngle() const;
    Vector2D GetRotator() const;

    virtual ~IVisibleObject() = default;

protected:
    Position pos_;
    double angle_;
    RenderLayers layer_;
};

class IFlexibleVisibleObject: public IVisibleObject {
public:
    IFlexibleVisibleObject() = default;
    IFlexibleVisibleObject(const Position& position, const double& angle = 0, const RenderLayers& layer = RenderLayers::Objects);

    template <typename Functor> /* Нахуячить проверок в стиле Ильи */
    void ChangeRenderLogic(Functor&& func) {
        render_logic_ = func;
    }

    void SetDefaultLogic();
    void RenderIt(Painter* painter) override; /* Наследники должны переопределять кастомный рендеринг*/
    virtual bool IsDisplayed() const override;

protected:
    virtual void RenderItCustom(Painter* painter);

    std::function<void(Painter*)> render_logic_;
};
