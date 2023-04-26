#include "Position.h"

Position::Position() : x_(0), y_(0) {
}

Position::Position(double x, double y) : x_(x), y_(y) {
}

void Position::SetCoordinates(double x, double y) {
    x_ = x;
    y_ = y;
}

Position::Position(const Position& other) : Position(other.x_, other.y_) {
}

std::pair<double, double> Position::GetCoordinates() const {
    return {x_, y_};
}

Vector2D Position::GetCoordinatesAsVector2D() const {
    return {x_, y_};
}

void Position::SetCoordinates(const Vector2D& other) {
    auto pair = other.GetCoordinates();
    x_ = pair.first;
    y_ = pair.second;
}

Position& Position::operator=(const Position& other) {
    x_ = other.x_;
    y_ = other.y_;
    return *this;
}

Position::Position(const Vector2D& other) {
    auto pair = other.GetCoordinates();
    x_ = pair.first;
    y_ = pair.second;
}

void Position::Translate(const Vector2D& other) {
    auto pair = other.GetCoordinates();
    x_ += pair.first;
    y_ += pair.second;
}