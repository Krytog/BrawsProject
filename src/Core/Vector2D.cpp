#include "Vector2D.h"

#include <cmath>

#define PRECISION 1e-9

Vector2D::Vector2D() : x_(0), y_(0) {
}

Vector2D::Vector2D(double x, double y) : x_(x), y_(y) {
}

Vector2D::Vector2D(const Vector2D& other) : Vector2D(other.x_, other.y_) {
}

void Vector2D::SetCoordinates(double x, double y) {
    x_ = x;
    y_ = y;
}

std::pair<double, double> Vector2D::GetCoordinates() const {
    return {x_, y_};
}

double Vector2D::Det(const Vector2D& first, const Vector2D& second) {
    return first.x_ * second.y_ - first.y_ * second.x_;
}

double Vector2D::ScalarMultiply(const Vector2D& first, const Vector2D& second) {
    return first.x_ * second.x_ + first.y_ * second.y_;
}

Vector2D& Vector2D::operator=(const Vector2D& other) = default;

double Vector2D::operator*(const Vector2D& other) const {
    return ScalarMultiply(*this, other);
}

Vector2D Vector2D::Right = Vector2D(1, 0);
Vector2D Vector2D::Left = Vector2D(-1, 0);
Vector2D Vector2D::Up = Vector2D(0, 1);
Vector2D Vector2D::Down = Vector2D(0, -1);

Vector2D Vector2D::operator*(const double coefficient) const {
    return {x_ * coefficient, y_ * coefficient};
}

Vector2D& Vector2D::operator*=(const double coefficient) {
    x_ *= coefficient;
    y_ *= coefficient;
    return *this;
}

Vector2D Vector2D::operator+(const Vector2D& other) const {
    return {x_ + other.x_, y_ + other.y_};
}

Vector2D Vector2D::operator-(const Vector2D& other) const {
    return {x_ - other.x_, y_ - other.y_};
}

Vector2D& Vector2D::operator+=(const Vector2D& other) {
    x_ += other.x_;
    y_ += other.y_;
    return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& other) {
    x_ -= other.x_;
    y_ -= other.y_;
    return *this;
}

double Vector2D::Length() const {
    return std::hypot(x_, y_);
}

void Vector2D::Normalize() {
    auto length = Length();
    if (length < PRECISION) {
        return;
    }
    x_ /= length;
    y_ /= length;
}

bool Vector2D::IsZero() const {
    return Length() < PRECISION;
}

Vector2D Vector2D::Rotate(const Vector2D &rotator) const {
    const double sin_of_angle = rotator.GetCoordinates().second / rotator.Length();
    double angle = std::asin(sin_of_angle);
    if (rotator.GetCoordinates().first < 0) {
        angle = M_PI - angle;
    }
    const double x = x_ * std::cos(angle) - y_ * std::sin(angle);
    const double y = x_ * std::sin(angle) + y_ * std::cos(angle);
    return Vector2D(x, y);
}

Vector2D Vector2D::Rotate(double angle) const {
    const double x = x_ * std::cos(angle) - y_ * std::sin(angle);
    const double y = x_ * std::sin(angle) + y_ * std::cos(angle);
    return Vector2D(x, y);
}

double Vector2D::GetAngle() const {
    const double sin_of_angle = y_ / Length();
    double angle = std::asin(sin_of_angle);
    if (x_ < 0) {
        angle = M_PI - angle;
    }
    return angle;
}