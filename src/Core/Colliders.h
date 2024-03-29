#pragma once

#define FRIEND_DECLARATION_FOR_IMPLEMENTATION                         \
    template <typename F, typename S>                                 \
    friend bool CheckImplementation(const F* first, const S* second); \
    template <typename F, typename S>                                 \
    friend std::optional<Position> IntersectionImplementation(const F* first, const S* second);

#include "Collider.h"

class CircleCollider : public Collider {
public:
    CircleCollider(const Position& position, double radius, Category category = Ordinary);

    void UpdatePosition(const Position& position) override;
    void Translate(const Vector2D& vector2D) override;
    bool IsInside(const Position& position) const override;

    std::optional<Position> GetIntersectionPosition(const Collider* other) const override;

private:
    bool Check(const Collider* other) const override;

    Position position_;
    double radius_;

    FRIEND_DECLARATION_FOR_IMPLEMENTATION
};

class RectangleCollider : public Collider {
public:
    RectangleCollider(const Position& position, double width, double height, Category category = Ordinary);

    void UpdatePosition(const Position& position) override;
    void Translate(const Vector2D& vector2D) override;
    bool IsInside(const Position& position) const override;

    std::optional<Position> GetIntersectionPosition(const Collider* other) const override;

private:
    bool Check(const Collider* other) const override;

    Position position_;
    double width_;
    double height_;

    FRIEND_DECLARATION_FOR_IMPLEMENTATION
};