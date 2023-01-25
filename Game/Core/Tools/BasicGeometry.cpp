#include "BasicGeometry.h"

#include <cmath>

const long double PRECISION = 1e-9;

bool IsBelongToSegment(const Position &point, const Position &seg_a, const Position &seg_b) {
    Vector2D ray1 = seg_a.GetCoordinatesAsVector2D() - seg_b.GetCoordinatesAsVector2D();
    Vector2D ray2 = seg_b.GetCoordinatesAsVector2D() - seg_a.GetCoordinatesAsVector2D();
    Vector2D pp1 = point.GetCoordinatesAsVector2D() - seg_b.GetCoordinatesAsVector2D();
    Vector2D pp2 = point.GetCoordinatesAsVector2D() - seg_a.GetCoordinatesAsVector2D();
    if (std::abs(ray1 * pp1 - ray1.Length() * pp1.Length()) < PRECISION &&
        std::abs(ray2 * pp2 - ray2.Length() * pp2.Length()) < PRECISION) {
        return true;
    }
    return false;
}

Line::Line(double a, double b, double c) : A_(a), B_(b), C_(c) {
}

double Line::operator()(const Position &point) const {
    return A_ * point.GetCoordinates().first + B_ * point.GetCoordinates().second + C_;
}

bool Line::AreParallel(const Line &first, const Line &second) {
    Vector2D a(first.A_, first.B_);
    Vector2D b(second.A_, second.B_);
    if (std::abs(Vector2D::Det(a, b)) < PRECISION) {
        return true;
    }
    return false;
}

bool Line::operator==(const Line &other) const {
    if (!AreParallel(*this, other)) {
        return false;
    }
    long double koef = 0;
    if (std::abs(A_) > PRECISION) {
        koef = other.A_ / A_;
    } else {
        if (std::abs(other.A_) > PRECISION) {
            return false;
        }
    }
    if (std::abs(B_) > PRECISION) {
        if (std::abs(koef - other.B_ / B_) > PRECISION) {
            return false;
        }
    } else {
        if (std::abs(other.B_) > PRECISION) {
            return false;
        }
    }
    if (std::abs(C_) > PRECISION) {
        if (std::abs(koef - other.C_ / C_) > PRECISION) {
            return false;
        }
    } else {
        if (std::abs(other.C_) > PRECISION) {
            return false;
        }
    }
    return true;
}

Position Line::Intersect(const Line &other) const {
    Vector2D a(A_, other.A_);
    Vector2D b(B_, other.B_);
    Vector2D c(C_, other.C_);
    double f = -Vector2D::Det(c, b) / Vector2D::Det(a, b);
    double s = -Vector2D::Det(a, c) / Vector2D::Det(a, b);
    return Position(f, s);
}

Line GetLine(const Position &f, const Position &s) {
    Vector2D dif = f.GetCoordinatesAsVector2D() - s.GetCoordinatesAsVector2D();
    double A = dif.GetCoordinates().second;
    double B = -dif.GetCoordinates().first;
    double C = -f.GetCoordinates().first * A - f.GetCoordinates().second * B;
    return Line(A, B, C);
}

bool DoesIntersect(const Position &b1, const Position &e1, const Position &b2, const Position &e2) {
    auto eq1 = GetLine(b1, e1);
    auto eq2 = GetLine(b2, e2);
    if (eq1(b2) * eq1(e2) > 0) {
        return false;
    }
    if (eq2(b1) * eq2(e1) > 0) {
        return false;
    }
    if (std::abs(eq1(b2)) < PRECISION && std::abs(eq1(e2)) < PRECISION) {
        if (!IsBelongToSegment(b1, b2, e2) && !IsBelongToSegment(e1, b2, e2)) {
            return false;
        }
    }
    return true;
}

Circle::Circle(const Position &center, double radius)
    : center_x_(center.GetCoordinates().first), center_y_(center.GetCoordinates().second), radius_(radius) {
}

Circle::Circle(const Position &a, const Position &b, const Position &c) {
    double coef = 2 * (a.GetCoordinates().first * (b.GetCoordinates().second - c.GetCoordinates().second) +
                       b.GetCoordinates().first * (c.GetCoordinates().second - a.GetCoordinates().second) +
                       c.GetCoordinates().first * (a.GetCoordinates().second - b.GetCoordinates().second));
    center_x_ = ((a.GetCoordinates().first * a.GetCoordinates().first +
                  a.GetCoordinates().second * a.GetCoordinates().second) *
                     (b.GetCoordinates().second - c.GetCoordinates().second) +
                 (b.GetCoordinates().first * b.GetCoordinates().first +
                  b.GetCoordinates().second * b.GetCoordinates().second) *
                     (c.GetCoordinates().second - a.GetCoordinates().second) +
                 (c.GetCoordinates().first * c.GetCoordinates().first +
                  c.GetCoordinates().second * c.GetCoordinates().second) *
                     (a.GetCoordinates().second - b.GetCoordinates().second)) /
                coef;
    center_y_ = -((a.GetCoordinates().first * a.GetCoordinates().first +
                   a.GetCoordinates().second * a.GetCoordinates().second) *
                      (b.GetCoordinates().first - c.GetCoordinates().first) +
                  (b.GetCoordinates().first * b.GetCoordinates().first +
                   b.GetCoordinates().second * b.GetCoordinates().second) *
                      (c.GetCoordinates().first - a.GetCoordinates().first) +
                  (c.GetCoordinates().first * c.GetCoordinates().first +
                   c.GetCoordinates().second * c.GetCoordinates().second) *
                      (a.GetCoordinates().first - b.GetCoordinates().first)) /
                coef;
    Vector2D center(center_x_, center_y_);
    radius_ = (center - a.GetCoordinatesAsVector2D()).Length();
}

std::pair<std::optional<Position>, std::optional<Position>> Circle::Intersect(const Line &line) const {
    auto nline = Line(line.A_, line.B_, line.C_ + line.A_ * center_x_ + line.B_ * center_y_);

    double x_ref = -nline.A_ * nline.C_ / (nline.A_ * nline.A_ + nline.B_ * nline.B_);
    double y_ref = -nline.B_ * nline.C_ / (nline.A_ * nline.A_ + nline.B_ * nline.B_);

    if (nline.C_ * nline.C_ - radius_ * radius_ * (nline.A_ * nline.A_ + nline.B_ * nline.B_) > PRECISION) {
        return {std::nullopt, std::nullopt};
    } else if (std::abs(nline.C_ * nline.C_ -
                        radius_ * radius_ * (nline.A_ * nline.A_ + nline.B_ * nline.B_)) < PRECISION) {
        return {Position(x_ref + center_x_, y_ref + center_y_),
                Position(x_ref + center_x_, y_ref + center_y_)};
    } else {
        double det = radius_ * radius_ - nline.C_ * nline.C_ / (nline.A_ * nline.A_ + nline.B_ * nline.B_);
        double coef = std::sqrt(det / (nline.A_ * nline.A_ + nline.B_ * nline.B_));
        return {Position(x_ref + nline.B_ * coef + center_x_, y_ref - nline.A_ * coef + center_y_),
                Position(x_ref - nline.B_ * coef + center_x_, y_ref + nline.A_ * coef + center_y_)};
    }
}

std::pair<std::optional<Position>, std::optional<Position>> Circle::Intersect(const Circle &other) const {
    auto xx = other.center_x_ - center_x_;
    auto yy = other.center_y_ - center_y_;
    auto line = Line(-2 * xx, -2 * yy, radius_ * radius_ - other.radius_ * other.radius_ + xx * xx + yy * yy);
    auto circ = Circle(Position(0, 0), radius_);
    auto pr = circ.Intersect(line);
    return {
        Position(pr.first->GetCoordinates().first + center_x_, pr.first->GetCoordinates().second + center_y_),
        Position(pr.second->GetCoordinates().first + center_x_,
                 pr.second->GetCoordinates().second + center_y_)};
}
