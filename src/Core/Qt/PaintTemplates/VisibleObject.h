#pragma once

#include "../../Position.h"
#include <QPainter>
#include <functional>

class IVisibleObject {
public:
    enum RenderLayers { BaseLayer, TexturesLayer, CharactersLayer };
public:
    IVisibleObject() = default;
    IVisibleObject(const Position& position, const RenderLayers& layer = RenderLayers::CharactersLayer);

    virtual void UpdatePosition(const Position &position);
    virtual void Translate(const Vector2D &vector2d);
    
    /* Не константный - можно будет менять свойства картинки(например цветовой канал) */
    virtual void RenderIt(QPainter* painter) = 0;
    RenderLayers GetLayer() const;
    virtual bool IsDisplayed() const = 0;

    Position GetPosition() const { 
        return pos_;
    }

    virtual ~IVisibleObject() = default;

protected:
    Position pos_;
    RenderLayers layer_;
};

class IFlexibleVisibleObject: public IVisibleObject {
public:
    IFlexibleVisibleObject() = default;
    IFlexibleVisibleObject(const Position& position, const RenderLayers& layer = RenderLayers::CharactersLayer);

    template <typename Functor> /* Нахуячить проверок в стиле Ильи */
    void ChangeRenderLogic(Functor&& func) {
        render_logic_ = func;
    }

    void SetDefaultLogic();
    void RenderIt(QPainter* painter) override; /* Наследники должны переопределять кастомный рендеринг*/
    virtual bool IsDisplayed() const override = 0;

protected:
    virtual void RenderItCustom(QPainter* painter) = 0;

    std::function<void(QPainter*)> render_logic_;
};
