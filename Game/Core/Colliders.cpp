#include "Colliders.h"
#include "Tools/BasicGeometry.h"

#include <cmath>
#include <stdexcept>

template <typename F, typename S>
bool CheckImplementation(const F *first, const S *second);

template <>
bool CheckImplementation<CircleCollider, CircleCollider>(const CircleCollider *first,
                                                         const CircleCollider *second) {
    const double radius_sum = first->radius_ + second->radius_;
    const Vector2D distance_vector =
        first->position_.GetCoordinatesAsVector2D() - second->position_.GetCoordinatesAsVector2D();
    const double distance = distance_vector.Length();
    return distance <= radius_sum;
}

template <>
bool CheckImplementation<CircleCollider, RectangleCollider>(const CircleCollider *circle,
                                                            const RectangleCollider *rectangle) {
    const Vector2D centers_difference = rectangle->position_.GetCoordinatesAsVector2D() -
                                        circle->position_.GetCoordinatesAsVector2D();
    const double centers_distance = centers_difference.Length();
    if (centers_distance <= circle->radius_) {
        return true;  // it covers the case when the rectangle is in the circle
    }
    const double x_distance = std::abs(rectangle->position_.GetCoordinates().first -
                                       circle->position_.GetCoordinates().first);
    const double y_distance = std::abs(rectangle->position_.GetCoordinates().second -
                                       circle->position_.GetCoordinates().second);
    if (x_distance <= rectangle->width_ / 2 && y_distance <= rectangle->height_ / 2) {
        return true;  // it covers the case when the circle is in the rectangle
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

    const double det = circle->radius_ * circle->radius_ - C * C / (A * A + B * B);
    const double coef = std::sqrt(det / (A * A + B * B));
    const Position first_intersection(x_ref + B * coef + circle->position_.GetCoordinates().first,
                                      y_ref - A * coef + circle->position_.GetCoordinates().second);
    const Position second_intersection(
        x_ref - B * coef + circle->position_.GetCoordinates().first,
        y_ref + A * coef + circle->position_.GetCoordinates().second);

    const double first_intersection_distance = (first_intersection.GetCoordinatesAsVector2D() -
                                                rectangle->position_.GetCoordinatesAsVector2D())
                                                   .Length();
    const double second_intersection_distance = (second_intersection.GetCoordinatesAsVector2D() -
                                                 rectangle->position_.GetCoordinatesAsVector2D())
                                                    .Length();
    const Position closest_point = first_intersection_distance <= second_intersection_distance
                                       ? first_intersection
                                       : second_intersection;

    const double x_distance_final = std::abs(rectangle->position_.GetCoordinates().first -
                                             closest_point.GetCoordinates().first);
    const double y_distance_final = std::abs(rectangle->position_.GetCoordinates().second -
                                             closest_point.GetCoordinates().second);
    return x_distance_final <= rectangle->width_ / 2 && y_distance_final <= rectangle->height_ / 2;
}

template <>
bool CheckImplementation<RectangleCollider, CircleCollider>(const RectangleCollider *rectangle,
                                                            const CircleCollider *circle) {
    return CheckImplementation<CircleCollider, RectangleCollider>(circle, rectangle);
}

template <>
bool CheckImplementation<RectangleCollider, RectangleCollider>(const RectangleCollider *first,
                                                               const RectangleCollider *second) {
    const double x_distance = std::abs(first->position_.GetCoordinates().first -
                                       second->position_.GetCoordinates().first);
    const double y_distance = std::abs(first->position_.GetCoordinates().second -
                                       second->position_.GetCoordinates().second);
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

bool CircleCollider::IsInside(const Position &position) const {
    auto coord = position.GetCoordinates();
    auto this_coord = position_.GetCoordinates();

    return ((coord.first - this_coord.first) * (coord.first - this_coord.first) +
            (coord.second - this_coord.second) * (coord.second - this_coord.second)) <=
           radius_ * radius_;
}

CircleCollider::CircleCollider(const Position &position, double radius, bool is_trigger)
    : position_(position), radius_(radius) {
    is_trigger_ = is_trigger;
}

bool CircleCollider::Check(const Collider *other) const {
    return CheckImplementationPrimary(this, other);
}

RectangleCollider::RectangleCollider(const Position &position, double width, double height,
                                     bool is_trigger)
    : position_(position), width_(width), height_(height) {
    is_trigger_ = is_trigger;
}

void RectangleCollider::UpdatePosition(const Position &position) {
    position_ = position;
}

void RectangleCollider::Translate(const Vector2D &vector2D) {
    position_.Translate(vector2D);
}

bool RectangleCollider::IsInside(const Position &position) const {
    auto coord = position.GetCoordinates();
    auto this_coord = position_.GetCoordinates();

    return (this_coord.first - width_ / 2 <= coord.first &&
            coord.first <= this_coord.first + width_ / 2) &&
           (this_coord.second - height_ / 2 <= coord.second &&
            coord.second <= this_coord.second + height_ / 2);
}

bool RectangleCollider::Check(const Collider *other) const {
    return CheckImplementationPrimary(this, other);
}

template <typename F, typename S>
std::optional<Position> IntersectionImplementation(const F *first, const S *second);

template <>
std::optional<Position> IntersectionImplementation<CircleCollider, CircleCollider>(
    const CircleCollider *first, const CircleCollider *second) {
    const double radius_sum = first->radius_ + second->radius_;
    const Vector2D distance_vector =
        first->position_.GetCoordinatesAsVector2D() - second->position_.GetCoordinatesAsVector2D();
    const double distance = distance_vector.Length();
    if (distance > radius_sum) {
        return std::nullopt;
    }
    const Circle circle1(first->position_, first->radius_);
    const Circle circle2(second->position_, second->radius_);
    const auto pair = circle1.Intersect(circle2);
    Vector2D midpoint;
    if (!pair.first || !pair.second) {
        throw std::runtime_error("Intersection of two circle failed!");
    }
    midpoint = pair.first->GetCoordinatesAsVector2D() + pair.second->GetCoordinatesAsVector2D();
    midpoint *= 0.5;
    return Position(midpoint);
}

template <>
std::optional<Position> IntersectionImplementation<CircleCollider, RectangleCollider>(
    const CircleCollider *circle, const RectangleCollider *rectangle) {
    const Vector2D centers_difference = rectangle->position_.GetCoordinatesAsVector2D() -
                                        circle->position_.GetCoordinatesAsVector2D();
    const double centers_distance = centers_difference.Length();
    if (centers_distance <= circle->radius_) {
        return rectangle->position_;  // it covers the case when the rectangle is in the circle
    }
    const double x_distance = std::abs(rectangle->position_.GetCoordinates().first -
                                       circle->position_.GetCoordinates().first);
    const double y_distance = std::abs(rectangle->position_.GetCoordinates().second -
                                       circle->position_.GetCoordinates().second);
    if (x_distance <= rectangle->width_ / 2 && y_distance <= rectangle->height_ / 2) {
        return circle->position_;  // it covers the case when the circle is in the rectangle
    }
    const auto line = GetLine(circle->position_, rectangle->position_);
    const Circle circle_geometry(circle->position_, circle->radius_);
    const auto pair = circle_geometry.Intersect(line);
    if (!pair.first || !pair.second) {
        throw std::runtime_error("Intersection of circle and rectangle failed!");
    }
    const double first_intersection_distance =
        (pair.first->GetCoordinatesAsVector2D() - rectangle->position_.GetCoordinatesAsVector2D())
            .Length();
    const double second_intersection_distance =
        (pair.second->GetCoordinatesAsVector2D() - rectangle->position_.GetCoordinatesAsVector2D())
            .Length();
    const Position closest_point = first_intersection_distance <= second_intersection_distance
                                       ? pair.first.value()
                                       : pair.second.value();

    const double x_distance_final = std::abs(rectangle->position_.GetCoordinates().first -
                                             closest_point.GetCoordinates().first);
    const double y_distance_final = std::abs(rectangle->position_.GetCoordinates().second -
                                             closest_point.GetCoordinates().second);
    if (x_distance_final <= rectangle->width_ / 2 && y_distance_final <= rectangle->height_ / 2) {
        return closest_point;
    }
    return std::nullopt;
}

template <>
std::optional<Position> IntersectionImplementation<RectangleCollider, CircleCollider>(
    const RectangleCollider *rectangle, const CircleCollider *circle) {
    return IntersectionImplementation<CircleCollider, RectangleCollider>(circle, rectangle);
}

template <>
std::optional<Position> IntersectionImplementation<RectangleCollider, RectangleCollider>(
    const RectangleCollider *first, const RectangleCollider *second) {
    const double x_distance = std::abs(first->position_.GetCoordinates().first -
                                       second->position_.GetCoordinates().first);
    const double y_distance = std::abs(first->position_.GetCoordinates().second -
                                       second->position_.GetCoordinates().second);
    if (!(x_distance <= (first->width_ + second->width_) / 2 &&
          y_distance <= (first->height_ + second->height_) / 2)) {
        return std::nullopt;
    }
    Vector2D first_rect[4];
    Vector2D second_rect[4];
    const Vector2D first_center = first->position_.GetCoordinatesAsVector2D();
    const Vector2D second_center = second->position_.GetCoordinatesAsVector2D();

    first_rect[0] = Vector2D(first_center.GetCoordinates().first - first->width_ / 2,
                             first_center.GetCoordinates().second - first->height_ / 2);
    first_rect[1] = Vector2D(first_center.GetCoordinates().first - first->width_ / 2,
                             first_center.GetCoordinates().second + first->height_ / 2);
    first_rect[2] = Vector2D(first_center.GetCoordinates().first + first->width_ / 2,
                             first_center.GetCoordinates().second + first->height_ / 2);
    first_rect[3] = Vector2D(first_center.GetCoordinates().first + first->width_ / 2,
                             first_center.GetCoordinates().second - first->height_ / 2);

    second_rect[0] = Vector2D(second_center.GetCoordinates().first - second->width_ / 2,
                              second_center.GetCoordinates().second - second->height_ / 2);
    second_rect[1] = Vector2D(second_center.GetCoordinates().first - second->width_ / 2,
                              second_center.GetCoordinates().second + second->height_ / 2);
    second_rect[2] = Vector2D(second_center.GetCoordinates().first + second->width_ / 2,
                              second_center.GetCoordinates().second + second->height_ / 2);
    second_rect[3] = Vector2D(second_center.GetCoordinates().first + second->width_ / 2,
                              second_center.GetCoordinates().second - second->height_ / 2);

    auto checker = [](const Vector2D &vertex, const RectangleCollider *ptr) {
        const double width = ptr->width_;
        const double height = ptr->height_;
        const double x_dist = vertex.GetCoordinates().first - ptr->position_.GetCoordinates().first;
        const double y_dist =
            vertex.GetCoordinates().second - ptr->position_.GetCoordinates().second;
        return x_dist <= (width + width) / 2 && y_dist <= (height + height) / 2;
    };

    size_t i = 0;
    for (; i < 4; ++i) {
        if (checker(first_rect[i], second)) {
            break;
        }
    }

    size_t j = 0;
    for (; j < 4; ++j) {
        if (checker(second_rect[j], first)) {
            break;
        }
    }

    if (i == 4) {
        return second->position_;  // The second is inside
    }
    if (j == 4) {
        return first->position_;  // The first is inside
    }

    Vector2D midpoint = first_rect[i] + second_rect[j];
    midpoint *= 0.5;
    return Position(midpoint);
}

template <typename T>
std::optional<Position> IntersectionImplementationPrimary(const T *self, const Collider *other) {
    if (const auto *ptr = dynamic_cast<const CircleCollider *>(other)) {
        return IntersectionImplementation<T, CircleCollider>(self, ptr);
    }
    if (const auto *ptr = dynamic_cast<const RectangleCollider *>(other)) {
        return IntersectionImplementation<T, RectangleCollider>(self, ptr);
    }
    throw std::runtime_error("Colliders dynamic cast failed!");
}

std::optional<Position> CircleCollider::GetIntersectionPosition(const Collider *other) const {
    return IntersectionImplementationPrimary(this, other);
}

std::optional<Position> RectangleCollider::GetIntersectionPosition(const Collider *other) const {
    return IntersectionImplementationPrimary(this, other);
}