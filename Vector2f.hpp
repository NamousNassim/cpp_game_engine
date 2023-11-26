#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <iostream>
#include <cmath>

class Vector2f
{
public:
    constexpr Vector2f();
    constexpr Vector2f(double a, double b);
    constexpr static Vector2f fromTrigonometricForm(double length, double angle);
    static double angleBetween(const Vector2f& a, const Vector2f& b);
    double length() const;
    constexpr double squareLength() const;
    double x, y;
};

constexpr Vector2f operator+ (const Vector2f& a, const Vector2f& b);
constexpr Vector2f operator- (const Vector2f& a, const Vector2f& b);
constexpr Vector2f operator* (const Vector2f& a, double b);
constexpr Vector2f operator* (double b, const Vector2f& a);
constexpr void operator +=(Vector2f a, Vector2f b);
constexpr Vector2f operator* (const Vector2f& a, const Vector2f& b);
constexpr bool operator== (const Vector2f& a, const Vector2f& b);
constexpr bool operator!= (const Vector2f& a, const Vector2f& b);

std::ostream& operator<<(std::ostream& a, Vector2f b);

constexpr Vector2f::Vector2f()
    :x(0.0), y(0.0) {}

constexpr Vector2f::Vector2f(double a, double b)
    :x(a), y(b) {}

constexpr Vector2f Vector2f::fromTrigonometricForm(double length, double angle)
{
    return Vector2f(cos(angle), sin(angle)) * length;
}

constexpr double Vector2f::squareLength() const
{
    return x*x + y*y;
}

constexpr Vector2f operator+ (const Vector2f& a, const Vector2f& b)
{
    return {a.x + b.x, a.y + b.y};
}

constexpr Vector2f operator- (const Vector2f& a, const Vector2f& b)
{
    return {a.x - b.x, a.y - b.y};
}

constexpr Vector2f operator* (const Vector2f& a, double b)
{
    return Vector2f(a.x * b, a.y * b);
}

constexpr Vector2f operator* (double b, const Vector2f& a)
{
    return Vector2f(a.x * b, a.y * b);
}

constexpr void operator +=(Vector2f a, Vector2f b)
{
    a.x += b.x;
    a.y += b.y;
}

constexpr Vector2f operator* (const Vector2f& a, const Vector2f& b)
{
    return Vector2f(a.x*b.x - a.y*b.y, a.x*b.y + a.y*b.x);
}

constexpr bool operator== (const Vector2f& a, const Vector2f& b)
{
    return a.x == b.x && a.y == b.y;
}

constexpr bool operator!= (const Vector2f& a, const Vector2f& b)
{
    return a.x != b.x || a.y != b.y;
}

#endif // VECTOR2_HPP
