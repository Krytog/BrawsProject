#include "Colliders.h"
#include "Tools/BasicGeometry.h"

#include <cmath>
#include <stdexcept>

const double PRECISION = 1e-9;

template <typename F, typename S>
bool CheckImplementation(const F* first, const S* second);

template <>
bool CheckImplementation<CircleCollider, CircleCollider>(const CircleCollider* first,
                                                         const CircleCollider* second) {
    const double radius_sum = first->radius_ + second->radius_;
    const Vector2D distance_vector =
        first->position_.GetCoordinatesAsVector2D() - second->position_.GetCoordinatesAsVector2D();
    const double distance = distance_vector.Length();
    return distance <= radius_sum;
}

template <>
bool CheckImplementation<CircleCollider, RectangleCollider>(const CircleCollider* circle,
                                                            const RectangleCollider* rectangle) {

    if (rectangle->IsInside(circle->position_)) {
        return true;
    }

    if (circle->IsInside(rectangle->position_)) {
        return true;
    }

    auto rect_center = rectangle->position_;
    Position vertices[4];
    vertices[0] = Position(rect_center.GetCoordinates().first - rectangle->width_ / 2,
                           rect_center.GetCoordinates().second - rectangle->height_ / 2);
    vertices[1] = Position(rect_center.GetCoordinates().first - rectangle->width_ / 2,
                           rect_center.GetCoordinates().second + rectangle->height_ / 2);
    vertices[2] = Position(rect_center.GetCoordinates().first + rectangle->width_ / 2,
                           rect_center.GetCoordinates().second + rectangle->height_ / 2);
    vertices[3] = Position(rect_center.GetCoordinates().first + rectangle->width_ / 2,
                           rect_center.GetCoordinates().second - rectangle->height_ / 2);

    Line left_line = GetLine(vertices[0], vertices[1]);
    Line upper_line = GetLine(vertices[1], vertices[2]);
    Line right_line = GetLine(vertices[2], vertices[3]);
    Line lower_line = GetLine(vertices[3], vertices[0]);

    Circle circ(circle->position_, circle->radius_);

    {
        auto pair = circ.Intersect(left_line);
        if (pair.first && rectangle->IsInside(*pair.first)) {
            return true;
        }
        if (pair.second && rectangle->IsInside(*pair.second)) {
            return true;
        }
    }

    {
        auto pair = circ.Intersect(upper_line);
        if (pair.first && rectangle->IsInside(*pair.first)) {
            return true;
        }
        if (pair.second && rectangle->IsInside(*pair.second)) {
            return true;
        }
    }

    {
        auto pair = circ.Intersect(right_line);
        if (pair.first && rectangle->IsInside(*pair.first)) {
            return true;
        }
        if (pair.second && rectangle->IsInside(*pair.second)) {
            return true;
        }
    }

    {
        auto pair = circ.Intersect(lower_line);
        if (pair.first && rectangle->IsInside(*pair.first)) {
            return true;
        }
        if (pair.second && rectangle->IsInside(*pair.second)) {
            return true;
        }
    }

    return false;
}

template <>
bool CheckImplementation<RectangleCollider, CircleCollider>(const RectangleCollider* rectangle,
                                                            const CircleCollider* circle) {
    return CheckImplementation<CircleCollider, RectangleCollider>(circle, rectangle);
}

template <>
bool CheckImplementation<RectangleCollider, RectangleCollider>(const RectangleCollider* first,
                                                               const RectangleCollider* second) {
    const double x_distance =
        std::abs(first->position_.GetCoordinates().first - second->position_.GetCoordinates().first);
    const double y_distance =
        std::abs(first->position_.GetCoordinates().second - second->position_.GetCoordinates().second);
    return x_distance <= (first->width_ + second->width_) / 2 &&
           y_distance <= (first->height_ + second->height_) / 2;
}

template <typename T>
bool CheckImplementationPrimary(const T* self, const Collider* other) {
    if (const auto* ptr = dynamic_cast<const CircleCollider*>(other)) {
        return CheckImplementation<T, CircleCollider>(self, ptr);
    }
    if (const auto* ptr = dynamic_cast<const RectangleCollider*>(other)) {
        return CheckImplementation<T, RectangleCollider>(self, ptr);
    }
    throw std::runtime_error("Colliders dynamic cast failed!");
    return false;
}

void CircleCollider::UpdatePosition(const Position& position) {
    position_ = position;
}

void CircleCollider::Translate(const Vector2D& vector2D) {
    position_.Translate(vector2D);
}

bool CircleCollider::IsInside(const Position& position) const {
    auto coord = position.GetCoordinates();
    auto this_coord = position_.GetCoordinates();

    return radius_ * radius_ - ((coord.first - this_coord.first) * (coord.first - this_coord.first) +
                                (coord.second - this_coord.second) * (coord.second - this_coord.second)) >=
           -PRECISION;
}

CircleCollider::CircleCollider(const Position& position, double radius, bool is_trigger)
    : position_(position), radius_(radius) {
    is_trigger_ = is_trigger;
}

bool CircleCollider::Check(const Collider* other) const {
    return CheckImplementationPrimary(this, other);
}

RectangleCollider::RectangleCollider(const Position& position, double width, double height, bool is_trigger)
    : position_(position), width_(width), height_(height) {
    is_trigger_ = is_trigger;
}

void RectangleCollider::UpdatePosition(const Position& position) {
    position_ = position;
}

void RectangleCollider::Translate(const Vector2D& vector2D) {
    position_.Translate(vector2D);
}

bool RectangleCollider::IsInside(const Position& position) const {
    auto coord = position.GetCoordinates();
    auto this_coord = position_.GetCoordinates();

    return width_ / 2 - std::abs(this_coord.first - coord.first) >= -PRECISION &&
           height_ / 2 - std::abs(this_coord.second - coord.second) >= -PRECISION;
}

bool RectangleCollider::Check(const Collider* other) const {
    return CheckImplementationPrimary(this, other);
}

template <typename F, typename S>
std::optional<Position> IntersectionImplementation(const F* first, const S* second);

template <>
std::optional<Position> IntersectionImplementation<CircleCollider, CircleCollider>(
    const CircleCollider* first, const CircleCollider* second) {
    const double radius_sum = first->radius_ + second->radius_;
    const Vector2D distance_vector =
        first->position_.GetCoordinatesAsVector2D() - second->position_.GetCoordinatesAsVector2D();
    const double distance = distance_vector.Length();
    if (distance > radius_sum) {
        return std::nullopt;
    }
    if (second->IsInside(first->position_)) {
        return first->position_;
    }
    if (first->IsInside(second->position_)) {
        return second->position_;
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
    const CircleCollider* circle, const RectangleCollider* rectangle) {

    if (rectangle->IsInside(circle->position_)) {
        return circle->position_;
    }

    if (circle->IsInside(rectangle->position_)) {
        return rectangle->position_;
    }

    auto rect_center = rectangle->position_;
    Position vertices[4];
    vertices[0] = Position(rect_center.GetCoordinates().first - rectangle->width_ / 2,
                           rect_center.GetCoordinates().second - rectangle->height_ / 2);
    vertices[1] = Position(rect_center.GetCoordinates().first - rectangle->width_ / 2,
                           rect_center.GetCoordinates().second + rectangle->height_ / 2);
    vertices[2] = Position(rect_center.GetCoordinates().first + rectangle->width_ / 2,
                           rect_center.GetCoordinates().second + rectangle->height_ / 2);
    vertices[3] = Position(rect_center.GetCoordinates().first + rectangle->width_ / 2,
                           rect_center.GetCoordinates().second - rectangle->height_ / 2);

    Line left_line = GetLine(vertices[0], vertices[1]);
    Line upper_line = GetLine(vertices[1], vertices[2]);
    Line right_line = GetLine(vertices[2], vertices[3]);
    Line lower_line = GetLine(vertices[3], vertices[0]);

    Circle circ(circle->position_, circle->radius_);

    {
        auto pair = circ.Intersect(left_line);
        if (pair.first && rectangle->IsInside(*pair.first)) {
            return pair.first;
        }
        if (pair.second && rectangle->IsInside(*pair.second)) {
            return pair.second;
        }
    }

    {
        auto pair = circ.Intersect(upper_line);
        if (pair.first && rectangle->IsInside(*pair.first)) {
            return pair.first;
        }
        if (pair.second && rectangle->IsInside(*pair.second)) {
            return pair.second;
        }
    }

    {
        auto pair = circ.Intersect(right_line);
        if (pair.first && rectangle->IsInside(*pair.first)) {
            return pair.first;
        }
        if (pair.second && rectangle->IsInside(*pair.second)) {
            return pair.second;
        }
    }

    {
        auto pair = circ.Intersect(lower_line);
        if (pair.first && rectangle->IsInside(*pair.first)) {
            return pair.first;
        }
        if (pair.second && rectangle->IsInside(*pair.second)) {
            return pair.second;
        }
    }

    return std::nullopt;
}

template <>
std::optional<Position> IntersectionImplementation<RectangleCollider, CircleCollider>(
    const RectangleCollider* rectangle, const CircleCollider* circle) {
    return IntersectionImplementation<CircleCollider, RectangleCollider>(circle, rectangle);
}

template <>
std::optional<Position> IntersectionImplementation<RectangleCollider, RectangleCollider>(
    const RectangleCollider* first, const RectangleCollider* second) {
    const double x_distance =
        std::abs(first->position_.GetCoordinates().first - second->position_.GetCoordinates().first);
    const double y_distance =
        std::abs(first->position_.GetCoordinates().second - second->position_.GetCoordinates().second);
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

    auto checker = [](const Vector2D& vertex, const RectangleCollider* ptr) {
        const double width = ptr->width_;
        const double height = ptr->height_;
        const double x_dist = std::abs(vertex.GetCoordinates().first - ptr->position_.GetCoordinates().first);
        const double y_dist =
            std::abs(vertex.GetCoordinates().second - ptr->position_.GetCoordinates().second);
        return x_dist <= width / 2 && y_dist <= height / 2;
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

    if (i < 4) {
        return Position(first_rect[i]);
    }

    if (j < 4) {
        return Position(second_rect[j]);
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
std::optional<Position> IntersectionImplementationPrimary(const T* self, const Collider* other) {
    if (const auto* ptr = dynamic_cast<const CircleCollider*>(other)) {
        return IntersectionImplementation<T, CircleCollider>(self, ptr);
    }
    if (const auto* ptr = dynamic_cast<const RectangleCollider*>(other)) {
        return IntersectionImplementation<T, RectangleCollider>(self, ptr);
    }
    throw std::runtime_error("Colliders dynamic cast failed!");
}

std::optional<Position> CircleCollider::GetIntersectionPosition(const Collider* other) const {
    return IntersectionImplementationPrimary(this, other);
}

std::optional<Position> RectangleCollider::GetIntersectionPosition(const Collider* other) const {
    return IntersectionImplementationPrimary(this, other);
}