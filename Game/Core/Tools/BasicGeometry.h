#pragma once

#include "../Position.h"

#include <utility>
#include <optional>

bool IsBelongToSegment(const Position& point, const Position& seg_a, const Position& seg_b);

bool DoesIntersect(const Position& b1, const Position& e1, const Position& b2, const Position& e2);

class Line {
public:
    Line(double a, double b, double c);

    Position Intersect(const Line& other) const;
    double operator()(const Position& point) const;
    bool operator==(const Line& other) const;

    static bool AreParallel(const Line& first, const Line& second);

private:
    friend class Circle;

    double A_;
    double B_;
    double C_;
};

Line GetLine(const Position& f, const Position& s);

class Circle {
public:
    Circle(const Position& center, double radius);
    Circle(const Position& a, const Position& b, const Position& c);

    std::pair<std::optional<Position>, std::optional<Position>> Intersect(const Line& line) const;
    std::pair<std::optional<Position>, std::optional<Position>> Intersect(
        const Circle& other) const;

private:
    long double center_x_;
    long double center_y_;
    long double radius_;
};
