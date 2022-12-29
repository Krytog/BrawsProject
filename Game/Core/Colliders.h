#pragma once

#include "Collider.h"

class CircleCollider : public Collider {
public:
  CircleCollider(const Position &position, double radius,
                 bool is_trigger = false);

  void UpdatePosition(const Position &position) override;
  void Translate(const Vector2D &vector2D) override;

private:
  bool Check(const Collider *other) const override;

  Position position_;
  double radius_;
};

class RectangleCollider : public Collider {
public:
  RectangleCollider(const Position &position, double width, double height,
                    bool is_trigger = false);

  void UpdatePosition(const Position &position) override;
  void Translate(const Vector2D &vector2D) override;

private:
  bool Check(const Collider *other) const override;

  Position position_;
  double width_;
  double height_;
};