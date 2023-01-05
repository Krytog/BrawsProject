#include "Colliders.h"

#include <cmath>
#include <stdexcept>

template <typename F, typename S>
bool CheckImplementation(const F *first, const S *second);

template <>
bool CheckImplementation<CircleCollider, CircleCollider>(
    const CircleCollider *first, const CircleCollider *second) {
  const double radius_sum = first->radius_ + second->radius_;
  const Vector2D distance_vector = first->position_.GetCoordinatesAsVector2D() -
                                   second->position_.GetCoordinatesAsVector2D();
  const double distance = distance_vector.Length();
  return distance <= radius_sum;
}

template <>
bool CheckImplementation<CircleCollider, RectangleCollider>(
    const CircleCollider *circle, const RectangleCollider *rectangle) {
  const Vector2D centers_difference =
      rectangle->position_.GetCoordinatesAsVector2D() -
      circle->position_.GetCoordinatesAsVector2D();
  const double centers_distance = centers_difference.Length();
  if (centers_distance <= circle->radius_) {
    return true; // it covers the case when the rectangle is in the circle
  }
  const double x_distance =
      std::abs(rectangle->position_.GetCoordinates().first -
               circle->position_.GetCoordinates().first);
  const double y_distance =
      std::abs(rectangle->position_.GetCoordinates().second -
               circle->position_.GetCoordinates().second);
  if (x_distance <= rectangle->width_ / 2 &&
      y_distance <= rectangle->height_ / 2) {
    return true; // it covers the case when the circle is in the rectangle
  }
  const double A = centers_difference.GetCoordinates().second;
  const double B = -centers_difference.GetCoordinates().first;
  const double C_old = -rectangle->position_.GetCoordinates().first * A -
                       rectangle->position_.GetCoordinates().second * B;
  // Line of the form Ax + By + C_old = 0;
  // We want to translate it so the circle is centered at (0, 0):
  const double C = C_old + A * circle->position_.GetCoordinates().first +
                   B * circle->position_.GetCoordinates().second;

  const double x_ref = -A * C / (A * A + B * B);
  const double y_ref = -B * C / (A * A + B * B);

  const double det =
      circle->radius_ * circle->radius_ - C * C / (A * A + B * B);
  const double coef = std::sqrt(det / (A * A + B * B));
  const Position first_intersection(
      x_ref + B * coef + circle->position_.GetCoordinates().first,
      y_ref - A * coef + circle->position_.GetCoordinates().second);
  const Position second_intersection(
      x_ref - B * coef + circle->position_.GetCoordinates().first,
      y_ref + A * coef + circle->position_.GetCoordinates().second);

  const double first_intersection_distance =
      (first_intersection.GetCoordinatesAsVector2D() -
       rectangle->position_.GetCoordinatesAsVector2D())
          .Length();
  const double second_intersection_distance =
      (second_intersection.GetCoordinatesAsVector2D() -
       rectangle->position_.GetCoordinatesAsVector2D())
          .Length();
  const Position closest_point =
      first_intersection_distance <= second_intersection_distance
          ? first_intersection
          : second_intersection;

  const double x_distance_final =
      std::abs(rectangle->position_.GetCoordinates().first -
               closest_point.GetCoordinates().first);
  const double y_distance_final =
      std::abs(rectangle->position_.GetCoordinates().second -
               closest_point.GetCoordinates().second);
  return x_distance_final <= rectangle->width_ / 2 &&
         y_distance_final <= rectangle->height_ / 2;
}

template <>
bool CheckImplementation<RectangleCollider, CircleCollider>(
    const RectangleCollider *rectangle, const CircleCollider *circle) {
  return CheckImplementation<CircleCollider, RectangleCollider>(circle,
                                                                rectangle);
}

template <>
bool CheckImplementation<RectangleCollider, RectangleCollider>(
    const RectangleCollider *first, const RectangleCollider *second) {
  const double x_distance = (first->position_.GetCoordinatesAsVector2D() -
                             second->position_.GetCoordinatesAsVector2D())
                                .Length();
  const double y_distance = (first->position_.GetCoordinatesAsVector2D() -
                             second->position_.GetCoordinatesAsVector2D())
                                .Length();
  return x_distance <= (first->width_ + second->width_) / 2 &&
         y_distance <= (first->height_ + second->height_) / 2;
}

template <typename T>
bool CheckImplementationPrimary(const T *self, const Collider *other) {
  if (const auto *ptr = dynamic_cast<const CircleCollider *>(other)) {
    return CheckImplementation<T, CircleCollider>(self, ptr);
  }
  if (const auto *ptr = dynamic_cast<const RectangleCollider *>(other)) {
    return CheckImplementation<T, RectangleCollider>(self, ptr);
  }
  throw std::runtime_error("Colliders dynamic cast failed!");
  return false;
}

void CircleCollider::UpdatePosition(const Position &position) {
  position_ = position;
}

void CircleCollider::Translate(const Vector2D &vector2D) {
  position_.Translate(vector2D);
}

CircleCollider::CircleCollider(const Position &position, double radius,
                               bool is_trigger)
    : position_(position), radius_(radius) {
  is_trigger_ = is_trigger;
}

bool CircleCollider::Check(const Collider *other) const {
  return CheckImplementationPrimary(this, other);
}

RectangleCollider::RectangleCollider(const Position &position, double width,
                                     double height, bool is_trigger)
    : position_(position), width_(width), height_(height) {
  is_trigger_ = is_trigger;
}

void RectangleCollider::UpdatePosition(const Position &position) {
  position_ = position;
}

void RectangleCollider::Translate(const Vector2D &vector2D) {
  position_.Translate(vector2D);
}

bool RectangleCollider::Check(const Collider *other) const {
  return CheckImplementationPrimary(this, other);
}