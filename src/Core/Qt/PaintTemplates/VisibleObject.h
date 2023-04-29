#pragma once

#include <qpainter.h>
#include "../../Position.h"
#include <QPainter>
#include <functional>

class IVisibleObject {
public:
    enum RenderLayers { BaseLayer, TexturesLayer, CharactersLayer };
public:
    IVisibleObject() = default;

    IVisibleObject(const Position& position);

    virtual void UpdatePosition(const Position &position);
    virtual void Translate(const Vector2D &vector2d);
    
    /* Не константный - можно будет менять свойства картинки(например цветовой канал) */
    virtual void RenderIt(QPainter* painter) = 0;

    virtual ~IVisibleObject() = default;

protected:
    Position pos_;
};

class IFlexibleVisibleObject: public IVisibleObject {
public:
    template <typename Functor>
    void ChangeRenderLogic(Functor&& func) {
        render_logic_ = func;
    }

    void SetDefaultLogic();
    bool IsCustomRendering() const;
    void RenderItCustom(const QPainter* painter) const;

    virtual void RenderIt(QPainter* painter) override = 0;

protected:
    std::function<void(const QPainter*)> render_logic_;
};
