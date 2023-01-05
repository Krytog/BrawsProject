#pragma once

#include "Vector2D.h"

class Position {
public:
    Position();
    Position(double x, double y);
    Position(const Position& other);
    explicit Position(const Vector2D& other);

    Vector2D GetCoordinatesAsVector2D() const;
    std::pair<double, double> GetCoordinates() const;
    void SetCoordinates(double x, double y);
    void SetCoordinates(const Vector2D& other);

    Position& operator=(const Position& other);

    void Translate(const Vector2D& other);

private:
    double x_;
    double y_;
};