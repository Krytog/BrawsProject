#pragma once

#include <utility>

class Vector2D {
public:
    Vector2D();
    Vector2D(double x, double y);
    Vector2D(const Vector2D& other);

    std::pair<double, double> GetCoordinates() const;
    void SetCoordinates(double x, double y);

    Vector2D& operator=(const Vector2D& other);

    double operator*(const Vector2D& other) const; //Scalar multiplication
    Vector2D operator*(double coefficient) const;
    Vector2D& operator*=(double coefficient);

    Vector2D operator+(const Vector2D& other) const;
    Vector2D operator-(const Vector2D& other) const;
    Vector2D& operator+=(const Vector2D& other);
    Vector2D& operator-=(const Vector2D& other);

    static double ScalarMultiply(const Vector2D& first, const Vector2D& second);
    static double Det(const Vector2D& first, const Vector2D& second);

    static Vector2D Up;
    static Vector2D Down;
    static Vector2D Left;
    static Vector2D Right;

private:
    double x_;
    double y_;
};
